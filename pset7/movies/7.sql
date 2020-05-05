SELECT movies.title, ratings.rating 
FROM movies JOIN ratings ON id = movie_id
WHERE year = 2010 and rating IS NOT NULL
ORDER BY rating DESC, title;