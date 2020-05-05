SELECT name FROM people
JOIN stars ON person_id = people.id
JOIN movies ON movie_id = movies.id
WHERE movies.title = "Toy Story";