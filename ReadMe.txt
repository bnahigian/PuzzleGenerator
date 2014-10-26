ReadMe

To generate a puzzle, I just randomly fill each cell (except the goal) with a value in the appropriate range.

After creating the puzzle, I use a modified version of dijkstra's to explore each node, find it's cost from start, what nodes it reaches, what nodes reach it, whether it is unique or not, etc.

After the forward dijkstra's, I use a backwads dijkstra's using the reacheby nodes (reverse edges). 

Using the information gathered above I can compute the score for the puzzle.


After the first "checkpuzzle()" function call, there exists 3 puzzle solutions. The first is m_sol which contains the current puzzle that is being operated on. This could be then randomly modified version or a randomly generated version.
The second is m_bestSol which contains the best version of the puzzle that we have gotten so far. This version is used to replace/ restore m_sol if a bad change is made. That also means when we keep a "bad" version of m_sol, this version is replaced.

Because this means that we might actually be leaving the best solution or one closest, I have a puzzle solution called althigh. This means "all-time high". Like the name implies, this contains the best version of the puzzle found and only updates if a better one is found.


Keeping Bad Puzzles:

I initially kept .3% of all "bad" puzzles- ones that made the m_bestSol worse. This worked very well for the 5x5 and I could get 101 or 102 most of the time. However, this did not work as well for 10x10- I maxed at about 350 but usually was around 300.

So I decided that similar to simulated annealing- which increases the probability of keeping a "bad" change based on how close to the "goal" state it is, I was going to be less likely to keep "bad" changes that were farther away from the best possible solution.
The equation I use to determine the keep probability for each function is calculated like so:

keepChance = (m_sol score / max possible score) / (m_bestSol score - m_sol score) / 20

This weights the score relative to the max possible score and how much "worse" it is than the best sol. The 20 at the end was to put it at an appropriately low probability similar to what i had been using before. The range tends to be between .2% and .9%. 

This new policy improved my 5x5 score slightly but caused a dramatic improvement in the 10x10. The average is about 410 and the max has been 426.


If you have any other questions about what I did, please don't hesitate to ask/ email me at bnahigia@usc.edu. Thanks- Brandon