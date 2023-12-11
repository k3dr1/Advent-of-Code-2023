## Advent-of-Code-2023

My solutions to Advent Of Code 2023!

I will try to not use my main languages (C++/Python) for as long as I can, to make it a little harder for myself)

##### The main objective is to not get filtered for as long as possible. The secondary objectives are:
* Improve my regex skills
* Learn Lua
* Improve my understanding of various algorithms

| Day       | Difficulty   | Notes                                            | Learned & Revised                  |
| --------- | ------------ | ------------------------------------------------ | ---------------------------------  |
| Day 1     | 4/10         | The second part was pretty tedious               | Aho-Corasick                       |
| Day 2     | 2/10         | Anticipated some probability stuff...            | ...                                |
| Day 3     | 5/10         | 2D grid, need to brush up on pathfinding         | ...                                |
| Day 4     | 2/10         | Extremely easy for some reason                   | Set Intersection Implementation    |
| Day 5     | 7/10         | Pretty interesting solution without bruteforce   | Interval splitting/reverse mapping |
| Day 6     | 1/10         | EASIEST                                          | ...                                |
| Day 7     | 3/10         | Quite tedious but very easy                      | ...                                |
| Day 8     | 4/10         | Would've been more interesting if the end states did not teleport you to a state adjacent to the start state. Currently you just take the lcm, but with that change, you would've had to solve Diophantine equations which are fun! | ...                               |
| Day 9     | 2/10         | Expected to see Gregory-Newton formula...        | Gregory-Newton formula             |
| Day 10    | 6/10         | Best day so far. I used Jordan Curve Theorem but  there are so many other solutions, it's great | Grid expansion (1x1 -> 3x3 or 2x2), Raycasting (evenodd) algorithm for a point inside the polygon, Winding number (nonzero) algorithm for the point inside the polygon, Jordan Curve Theorem, Stack based floodfill, Scan fill, Pick's Theorem, Shoelace Formula (which is apparently a special case of Green's Theorem) |
| Day 11    | 3/10         | Apparently can be done in O(n) since dimensions are completely independent when taking manhattan distance. My solution is O(n<sup>2</sup>) but even that was enough to get the answer instantly, so pretty easy  | ...                                |
