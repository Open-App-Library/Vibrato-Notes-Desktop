drop table if exists heroes;
drop table if exists vehicles;

create table heroes (name, power);
create table vehicles (name, color, speed);

insert into heroes values
("Batman", "Money"),
("Spiderman", "Webs");

insert into vehicles values
("Ferrari", "red", 349),
("Apache Helicopter", "camo", 227),
("Apache Webserver", "gray", 30000),
("Skateboard", "brown", 7);
