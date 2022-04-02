select c2.class from(
    (select classes.class, count(*) from 
    ships, outcomes, classes 
    where ships.class = classes.class
    and  ships.name = outcomes.ship 
    and outcomes.outcomes = 'sunk' group by classes.class) 
    as cls(classing, count), 
    select c1.class, count(*) 
    from ships s1, outcomes o1, classes c1 
    where s1.class = c1.class 
    and  s1.name = o1.ship 
    and o1.outcomes = 'sunk' 
    group by c1.class as cls2(class, count) 
    where cls2.class <> cls.class and 
    cls2.count > cls.count);