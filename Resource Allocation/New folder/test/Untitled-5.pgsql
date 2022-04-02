
select distinct c9.class from classes c9, ships s9, outcomes o9 where o9.ship  = s9.name and c9.class = s9.class and o9.outcomes  = 'sunk'

-- select c9.class from classes c9 
and
not exists
 ( 
 
 select * from 
    (
        select classes.class, count(*) from classes, ships, outcomes 
        where ships.name  = outcomes.ship 
        and ships.class = classes.class
        and classes.class = c9.class
        and outcomes.outcomes = 'sunk' 
        group by classes.class
    ) as cls (class, count)
    , 
    (
        select c2.class, count(*) from ships s2,outcomes o2,classes c2 
        where s2.name  = o2.ship 
        and c2.class = s2.class
        and o2.outcomes = 'sunk' 
        group by c2.class
    )
    as cls2(class, count)
    where cls.count < cls2.count
 );
