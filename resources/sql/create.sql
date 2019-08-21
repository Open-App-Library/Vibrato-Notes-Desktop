CREATE TABLE IF NOT EXISTS notes (
  uuid STRING UNIQUE,
  mimetype TEXT,
  encoding TEXT,
  title TEXT,
  text TEXT,
  date_created DATETIME,
  date_modified DATETIME,
  notebook STRING REFERENCES notebooks(uuid),
  favorited BOOLEAN,
  encrypted BOOLEAN,
  trashed BOOLEAN
);

CREATE TABLE if NOT EXISTS notebooks (
  uuid INTEGER unique,
  title TEXT,
  date_modified DATETIME,
  parent STRING REFERENCES notebooks(uuid),
  row INTEGER,
  encrypted BOOLEAN
);

CREATE TABLE IF NOT EXISTS tags (
  uuid INTEGER UNIQUE,
  title TEXT,
  date_modified DATETIME,
  row INTEGER,
  encrypted BOOLEAN
);

-- Many-to-many relationship between notes and tags
CREATE TABLE IF NOT EXISTS notes_tags (
  note STRING REFERENCES notes(uuid),
  tag INTEGER REFERENCES tags(uuid)
);
