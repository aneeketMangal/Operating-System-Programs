select c1.class, c1.country from classes c1 where exists (select * from ships s2, classes c2 where c2.class = s2.class and c1.class = c2.class) and not exists (select * from ships s3, outcomes o3, classes c3 where c3.class = c1.class and c3.class = s3.class and o3.ship = s3.name);
select distinct c1.class, c1.country, s1.name, s1.yearlaunched from ships s1, classes c1 where s1.class = c1.class and not exists(select * from ships s2 where s2.yearlaunched < s1.yearlaunched and s2.class = c1.class);
select c1.class, c1.country, ( select count(*) from ships,outcomes where ships.name = outcomes.ship and ships.class = c1.class) from classes c1;
select * from battles where not exists (select * from ships, outcomes, classes where ships.name = outcomes.ship AND  outcomes.battle = battles.name and ships.class = classes.class and classes.country<>'INDIA' and classes.country<>'ITALY');



select distinct classes.class
from ships, classes
where ships.class=classes.class 
and ships.class not in  
(
select class
from outcomes,ships
where outcomes.ship = ships.name
);

select classes.class, classes.country,ships.name	
from ships, classes, 
	(select class, min(yearlaunched)
	from ships 
	group by class) as mincy(cl,my) 
where 
	ships.class=classes.class
	and ships.class=mincy.cl
	and ships.yearlaunched=mincy.my;

select class, country,
	(select count(*)
	from outcomes o1, ships s1
	where o1.ship=s1.name
	and s1.class=c1.class
	)
from classes c1 except 
select c1.class, c1.country, ( select count(*) from ships,outcomes where ships.name = outcomes.ship and ships.class = c1.class) from classes c1;
;

select *
from 
battles
where not exists
(
select *
from outcomes o1,ships s1, classes c1
where 
o1.ship=s1.name 
and s1.class=c1.class
and o1.battle= battles.name
and c1.country<>'ITALY'
and c1.country<>'INDIA'
);
-- select c9.class from classes c9
-- -- ; 
-- where not exists
-- (
--     select * from 
--     (
--         select count(*) as countL from ships, outcomes 
--         where ships.name  = outcomes.ship 
--         and ships.class = c9.class
--         and outcomes.outcomes = 'sunk' 
--     )
--     , 
--     (
--         select c2.class, count(*) from ships s2,outcomes o2,classes c2 
--         where s2.name  = o2.ship 
--         and c2.class = s2.class
--         and c2.class <> c9.class
--         and o2.outcomes = 'sunk' 
--         group by c2.class
--     )
--     as cls2(class, count)

--     where countL < cls2.count
-- )
-- ;