select distinct t1.ins_id, (
    select max(ct.count)
    from (
        select tee2.course_id, tee2.semester, tee2.year, count(*)
        from teaches tee2, takes t2
        where tee2.ins_id = t1.ins_id
        and (tee2.course_id, tee2.semester, tee2.year) = (t2.course_id, t2.semester, t2.year)
        group by tee2.course_id, tee2.semester, tee2.year
        
    ) as ct(id, sem, year, count) where ct.count is not NULL
) from teaches t1;