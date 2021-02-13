module HW1 (
    form,
    constGrid,
    flatten,
    access,
    slice,
    vcat,
    hcat,
    without,
    matches2d
) where

-- do not modify the module declaration above!
-- this will ensure that you cannot load (compile)
-- the module without implementing all of the functions.

-- If you have functions you do not want to implement,
-- leave them as undefined or make them have another
-- default value. If you fully remove any of their definitions,
-- that will be a compilation error during evaluation,
-- and you will be eligible for (yay!) a 5 point deduction
-- (that's bad for your grade). Runtime errors in your code 
-- (rather than compilation errors) are acceptable and will simply
-- result in you getting zero from the specific test case causing
-- an error.

-------------------------
-- Fellowship of the Grid (25, 5, 5, 5 points)
form :: [a] -> (Int, Int) -> [[a]] 
form [] _ = []
form list (1,_) = [list]
form list (n,length) = take length list: form (drop length list) (n-1,length)

constGrid :: a -> (Int, Int) -> [[a]]
constGrid el (f,s) = replicate f (replicate s el)

flatten :: [[a]] -> [a]
flatten grid = [x | s <- grid, x<- s]

access :: [[a]] -> (Int, Int) -> a
access grid (f,s) = grid !! f !! s
----------------------------
-- The Two Signatures (10, 5, 5, 10 points)
slice :: [[a]] -> (Int, Int) -> (Int, Int) -> [[a]]
slice grid (i1,i2) (j1,j2) = [sliceList x (j1,j2)| x<- cropped]
  where cropped = drop i1 (take i2 grid)
        sliceList list (j1,j2) = drop j1 (take j2 list)

vcat :: [[a]] -> [[a]] -> [[a]]
vcat grid1 grid2 = grid1 ++ grid2

hcat :: [[a]] -> [[a]] -> [[a]]
hcat grid1 grid2 = zipWith (++) grid1 grid2

without :: [[a]] -> (Int, Int) -> (Int, Int) -> [[a]]
without grid (i1,i2) (j1,j2) = [withoutList x (j1,j2)| x<- cropped]
  where cropped = (take i1 grid) ++ (drop i2 grid)
        withoutList list (j1,j2) = (take j1 list) ++ (drop j2 list)
----------------------------
-- Return of the Non-trivial (30 points, 15 subject to runtime constraints)
matches2d :: Eq a => [[a]] -> [[a]] -> [(Int, Int)]
matches2d grid pattern = matHelper grid pattern 0

matHelper :: Eq a => [[a]] -> [[a]] -> Int -> [(Int, Int)]
matHelper grid pattern n
    | lenPat > lenGrid = []
    | otherwise = list ++ matHelper newGrid pattern (n+1)
    where lenPat = length $ pattern !! 0
          lenGrid = length $ grid !! 0
          list = zip (mHelper2 grid pattern 0) (repeat n)
          newGrid = [drop 1 x| x<- grid]

mHelper2 :: Eq a => [[a]] -> [[a]] -> Int -> [Int]
mHelper2 grid pattern n
    | lenPat > lenGrid = []
    | otherwise = if compare1 grid pattern then n: rest else rest
    where lenPat = length pattern
          lenGrid = length grid
          rest = mHelper2 (drop 1 grid) pattern (n+1)

compare1 :: Eq a => [[a]] -> [[a]] -> Bool
compare1 _ [] = True
compare1 (g_head:g_rest) (p_head:p_rest) = if compare2 g_head p_head then compare1 g_rest p_rest else False

compare2 :: Eq a => [a] -> [a] -> Bool
compare2 _ [] = True
compare2 (l1_h:l1_r) (l2_h:l2_r) = if l1_h == l2_h then compare2 l1_r l2_r else False
----------------------------
-- What is undefined? Just a value that will cause an error
-- when evaluated, from the GHC implementation:
-- undefined = error "Prelude.undefined"
-- But it allows your module to be compiled
-- since the function definitions will exist.
