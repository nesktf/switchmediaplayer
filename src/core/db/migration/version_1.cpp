#include "core/db/migration/version_1.hpp"

#include <borealis.hpp>

namespace core {
namespace db {
bool migrationV1(util::SQLite* db) {
  bool ret;

  ret = db->prepareExec(R"(
    CREATE TABLE [Sources] (
      [ID] INTEGER NOT NULL PRIMARY KEY,
      [Path] TEXT NOT NULL
    );
  )");
  if (!ret) {
    brls::Logger::error("[Database] Failed to create Source Table");
    return false;
  }

  ret = db->prepareExec(R"(
    CREATE TABLE [Artists] (
      [ID] INTEGER NOT NULL PRIMARY KEY,
      [Name] TEXT UNIQUE NOT NULL
    );
  )");
  if (!ret) {
    brls::Logger::error("[Database] Failed to create Artists Table");
    return false;
  }

  ret = db->prepareExec(R"(
    CREATE TABLE [Genres] (
      [ID] INTEGER NOT NULL PRIMARY KEY,
      [Name] TEXT UNIQUE NOT NULL
    );
  )");
  if (!ret) {
    brls::Logger::error("[Database] Failed to create Genres Table");
    return false;
  }

  ret = db->prepareExec(R"(
    CREATE TABLE [Albums] (
      [ID] INTEGER NOT NULL PRIMARY KEY,
      [Title] TEXT UNIQUE NOT NULL,
      [Artists] TEXT,
      [Cover] TEXT
    );
  )");
  if (!ret) {
    brls::Logger::error("[Database] Failed to create Albums Table");
    return false;
  }

  // TODO Make genres point to music, instead of music pointing to genres
  ret = db->prepareExec(R"(
    CREATE TABLE [Music] (
      [ID] INTEGER NOT NULL PRIMARY KEY,
      [Path] TEXT NOT NULL,
      [Title] TEXT NOT NULL,
      [Length] INTEGER NOT NULL,
      [Track] INTEGER NOT NULL,
      [Date] INTEGER,
      [ArtistID] INTEGER,
      [GenreID] INTEGER,
      [AlbumID] INTEGER,
      [SourceID] INTEGER NOT NULL,
      FOREIGN KEY (SourceID) REFERENCES Sources(ID) ON DELETE CASCADE,
      FOREIGN KEY (ArtistID) REFERENCES Artists(ID),
      FOREIGN KEY (GenreID) REFERENCES Genres(ID),
      FOREIGN KEY (AlbumID) REFERENCES Albums(ID)
    );
  )");
  if (!ret) {
    brls::Logger::error("[Database] Failed to create Music Table");
    return false;
  }

  // Trigger for deleting artists when no music references them
  ret = db->prepareExec(R"(
    CREATE TRIGGER delete_artists AFTER DELETE ON Music WHEN NOT EXISTS (
      SELECT 1 
      FROM Music
      WHERE Music.ArtistID = OLD.ArtistID
    )
    BEGIN
      DELETE FROM Artists WHERE Artists.ID = OLD.ArtistID;
    END;
  )");
  if (!ret) {
    brls::Logger::error("[Database] Failed to create delete_artists Trigger");
    return false;
  }

  // Same as above but for genres
  ret = db->prepareExec(R"(
    CREATE TRIGGER delete_genres AFTER DELETE ON Music WHEN NOT EXISTS (
      SELECT 1 
      FROM Music
      WHERE Music.GenreID = OLD.GenreID
    )
    BEGIN
      DELETE FROM Genres WHERE Genres.ID = OLD.GenreID;
    END;
  )");
  if (!ret) {
    brls::Logger::error("[Database] Failed to create delete_genres Trigger");
    return false;
  }

  // Same as above but for albums
  ret = db->prepareExec(R"(
    CREATE TRIGGER delete_albums AFTER DELETE ON Music WHEN NOT EXISTS (
      SELECT 1 
      FROM Music
      WHERE Music.AlbumID = OLD.AlbumID
    )
    BEGIN
      DELETE FROM Albums WHERE Albums.ID = OLD.AlbumID;
    END;
  )");
  if (!ret) {
    brls::Logger::error("[Database] Failed to create delete_albums Trigger");
    return false;
  }

  ret = db->prepareExec(R"(
    CREATE TABLE [Playlists] (
      [ID] INTEGER NOT NULL PRIMARY KEY,
      [Title] TEXT UNIQUE NOT NULL
    );
  )");
  if (!ret) {
    brls::Logger::error("[Database] Failed to create Playlists Table");
    return false;
  }

  ret = db->prepareExec(R"(
    CREATE TABLE [Video] (
      [ID] INTEGER NOT NULL PRIMARY KEY,
      [Path] TEXT NOT NULL,
      [Title] TEXT NOT NULL,
      [Length] INTEGER NOT NULL,
      [SourceID] INTEGER NOT NULL,
      FOREIGN KEY (SourceID) REFERENCES Sources(ID) ON DELETE CASCADE
    );
  )");
  if (!ret) {
    brls::Logger::error("[Database] Falied to create Video Table");
    return false;
  }

  ret = db->prepareExec(R"(
    CREATE TABLE [PlaylistContent] (
      [ID] INTEGER NOT NULL PRIMARY KEY,
      [MusicID] INTEGER NOT NULL,
      [PlayslistID] INTEGER NOT NULL,
      FOREIGN KEY (MusicID) REFERENCES Music(ID) ON DELETE CASCADE,
      FOREIGN KEY (PlayslistID) REFERENCES Playlists(ID) ON DELETE CASCADE
    );
  )");
  if (!ret) {
    brls::Logger::error("[Database] Failed to create PlaylistContent Table");
    return false;
  }

  ret = db->prepareExec(R"(
    CREATE TABLE [History] (
      [ID] INTEGER NOT NULL PRIMARY KEY,
      [Date] INTEGER NOT NULL
    );
  )");
  if (!ret) {
    brls::Logger::error("[Database] Failed to create History Table");
    return false;
  }

  ret = db->prepareExec(R"(
    CREATE TABLE [MusicHistory] (
      [ID] INTEGER NOT NULL PRIMARY KEY,
      [MusicID] INTEGER NOT NULL,
      [HistoryID] INTEGER NOT NULL,
      FOREIGN KEY (HistoryID) REFERENCES History(ID) ON DELETE CASCADE,
      FOREIGN KEY (MusicID) REFERENCES Music(ID) ON DELETE CASCADE
    );
  )");
  if (!ret) {
    brls::Logger::error("[Database] Failed to create MusicHistory Table");
    return false;
  }

  ret = db->prepareExec(R"(
    CREATE TABLE [VideoHistory] (
      [ID] INTEGER NOT NULL PRIMARY KEY,
      [VideoID] INTEGER NOT NULL,
      [HistoryID] INTEGER NOT NULL,
      FOREIGN KEY (HistoryID) REFERENCES History(ID) ON DELETE CASCADE,
      FOREIGN KEY (VideoID) REFERENCES Video(ID) ON DELETE CASCADE
    );
  )");
  if (!ret) {
    brls::Logger::error("[Database] Failed to create VideoHistory Table");
    return false;
  }

  ret = db->prepareExec("UPDATE Vars SET Value=1 WHERE Name='db_version';");
  if (!ret) {
    brls::Logger::error("[Database] Failed to update database version");
    return false;
  }

  return true;
}
}
}

