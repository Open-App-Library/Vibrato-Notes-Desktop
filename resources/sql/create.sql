CREATE TABLE IF NOT EXISTS notes (
  sync_hash STRING UNIQUE,
  title TEXT,
  text TEXT,
  date_created DATETIME,
  date_modified DATETIME,
  notebook STRING REFERENCES notebooks(sync_hash),
  favorited BOOLEAN,
  encrypted BOOLEAN,
  trashed BOOLEAN
);

CREATE TABLE if NOT EXISTS notebooks (
  sync_hash INTEGER unique,
  title TEXT,
  date_modified DATETIME,
  parent STRING REFERENCES notebooks(sync_hash),
  row INTEGER,
  encrypted BOOLEAN
);

CREATE TABLE IF NOT EXISTS tags (
  sync_hash INTEGER UNIQUE,
  title TEXT,
  date_modified DATETIME,
  row INTEGER,
  encrypted BOOLEAN
);

-- Many-to-many relationship between notes and tags
CREATE TABLE IF NOT EXISTS notes_tags (
  note STRING REFERENCES notes(sync_hash),
  tag INTEGER REFERENCES tags(sync_hash)
);
