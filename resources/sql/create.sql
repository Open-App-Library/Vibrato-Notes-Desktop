create table if not exists notes (
sync_hash integer unique,
id integer primary key autoincrement,
title text,
text text,
date_created datetime,
date_modifed timetime,
favorited boolean,
notebook integer references notebooks(id),
trashed boolean
);

create table if not exists notebooks (
sync_hash integer unique,
id integer primary key autoincrement, -- my cool thing
title text,
parent integer references notebooks(id)
);

create table if not exists tags (
sync_hash integer unique,
id integer primary key autoincrement,
title text
);

-- Many-to-many relationship between notes and tags
create table if not exists notes_tags (
note integer references notes(id),
tag integer references tags(id)
);
