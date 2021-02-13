module HW2 (
    parse, -- reexport for easy terminal use
    foldAndPropagateConstants,
    assignCommonSubexprs,
    reducePoly
) where

import Expression
import Parser

-- Do not change the module definition and imports above! Feel free
-- to modify the parser, but please remember that it is just a helper
-- module and is not related to your grade. You should only submit
-- this file. Feel free to import other modules, especially Data.List!

extract :: ExprV -> Int
extract (Leaf (Constant x)) = x

makeleaf:: Int -> ExprV
makeleaf x = Leaf (Constant x)

found :: String -> [(String, Int)] -> (Bool, Int)
found name [] = (False, 0)
found name (x:xs)
    | fst x == name = (True, snd x)
    | otherwise = found name xs

compute :: ExprV -> [(String, Int)] -> (Bool, ExprV)
compute element@(Leaf (Variable name)) list = if fst a then (True, makeleaf (snd a)) else (False, element)
    where a = found name list
compute element@(Leaf (Constant x)) _ = (True, element)
compute (UnaryOperation _ x) list = if fst a then (True, val) else (False, (UnaryOperation Minus (snd a)))
    where a = compute x list
          val = makeleaf $ -extract (snd a)
compute (BinaryOperation oper x1 x2) list
    | fst a1 && fst a2 = if oper == Plus then (True, valPlus) else (True, valTimes)
    | otherwise = (False, (BinaryOperation oper (snd a1) (snd a2)))
    where a1 = compute x1 list
          a2 = compute x2 list
          valPlus = makeleaf $ extract (snd a1) + extract (snd a2)
          valTimes = makeleaf $ extract (snd a1) * extract (snd a2)

foldHelper :: [(String, ExprV)] -> [(String, Int)] -> [(String, ExprV)]
foldHelper [] _ = []
foldHelper (element:rest) list
    | fst result = (fst element, snd result) : foldHelper rest newlist
    | otherwise = (fst element, snd result) : foldHelper rest list
    where result = compute (snd element) list
          newlist = (fst element, extract (snd result)):list

foldAndPropagateConstants :: [(String, ExprV)] -> [(String, ExprV)]
foldAndPropagateConstants x = foldHelper x []

-------------------------------------------------

isLeaf :: ExprV -> Bool
isLeaf (Leaf _) = True
isLeaf _ = False

organizer :: ExprV -> [(ExprV, Int)] -> [(ExprV, Int)]
organizer x [] = [(x,1)]
organizer x (el:rest)
    | x == fst el = (x,newvalue):rest
    | otherwise = el:(organizer x rest)
    where newvalue = snd el + 1

assignHelper :: ExprV -> [(ExprV, Int)] -> [(ExprV, Int)]
assignHelper (Leaf _) _ = []
assignHelper element@(UnaryOperation _ x) list
    | isLeaf x = organizer element list
    | otherwise = assignHelper x list
assignHelper element@(BinaryOperation oper x1 x2) list
    | cond1 && cond2 = organizer element list
    | not cond1 = if cond2 then value else assignHelper x2 value
    | otherwise = assignHelper x2 list
    where cond1 = isLeaf x1
          cond2 = isLeaf x2
          value = assignHelper x1 list

exchangerHelper :: ExprV -> ExprV -> Int -> ExprV
exchangerHelper element@(Leaf _) _ _ = element
exchangerHelper element@(UnaryOperation _ x) wanted num
    | element == wanted = newvar
    | otherwise = UnaryOperation Minus (exchangerHelper x wanted num)
    where newvar = Leaf $ Variable $ "$"++ show num
exchangerHelper element@(BinaryOperation oper x1 x2) wanted num
    | element == wanted = newvar
    | otherwise = BinaryOperation oper (exchangerHelper x1 wanted num) (exchangerHelper x2 wanted num)
    where newvar = Leaf $ Variable $ "$"++ show num

exchanger :: ExprV -> [ExprV] -> Int -> ExprV
exchanger element [] _ = element
exchanger element (first:rest) num = exchanger (exchangerHelper element first num) rest (num+1)

myreduce :: [(ExprV, Int)] -> [ExprV]
myreduce [] = []
myreduce (el:rest)
    | snd el > 1 = (fst el):(myreduce rest)
    | otherwise = myreduce rest

makeit :: [ExprV] -> Int -> [(String, ExprV)]
makeit [] _ = []
makeit (el:rest) num = ("$"++show num, el):(makeit rest (num+1))

assignWithNum :: Int -> ExprV -> ([(String, ExprV)],ExprV)
assignWithNum num x
    | list == [] = ([],x)
    | otherwise = (expList++(fst value), snd value)
    where value = assignWithNum (num+length list) result
          result = exchanger x list num
          expList = makeit list num
          list = myreduce rawlist
          rawlist = assignHelper x []

assignCommonSubexprs :: ExprV -> ([(String, ExprV)], ExprV)
assignCommonSubexprs = assignWithNum 0

-------------------------------------------------

topla :: [Int] -> [Int] -> [Int]
topla [] [] = []
topla x [] = x
topla [] y = y
topla (x:xs) (y:ys) = (x+y):(topla xs ys)

carp :: [Int] -> [Int] -> [Int]
carp [] _ = []
carp [x] y = map (x*) y
carp (x:xs) y = topla (map (x*) y) (0:(carp xs y))


reduceHelper :: ExprV -> [Int]
reduceHelper (Leaf (Constant x)) = [x]
reduceHelper (Leaf (Variable var)) = [0,1]
reduceHelper (UnaryOperation _ x) = map ((-1)*) (reduceHelper x)
reduceHelper (BinaryOperation oper x1 x2)
    | oper == Plus = topla (reduceHelper x1) (reduceHelper x2)
    | oper == Times = carp (reduceHelper x1) (reduceHelper x2)

func :: ExprV -> String
func (Leaf (Variable var)) = var
func (Leaf (Constant x)) = ""
func (UnaryOperation _ x) = func x
func (BinaryOperation _ x1 x2) = if result == "" then func x2 else result
    where result = func x1

terim :: Int -> Int -> String -> ExprV
terim x 0 _ = Leaf $ Constant x
terim x 1 name
    | x == 1 = Leaf $ Variable name
    | x == -1 = UnaryOperation Minus (Leaf $ Variable name)
terim x num name = BinaryOperation Times (terim x (num-1) name) (Leaf $ Variable name)

numofNonZero :: [Int] -> Int
numofNonZero [] = 0
numofNonZero (x:xs)
    | x == 0 = numofNonZero xs
    | otherwise = 1 + numofNonZero xs

dropZeros :: [Int] -> [Int]
dropZeros [] = []
dropZeros list
    | (last list) == 0 = dropZeros (init list)
    | otherwise = list

makeExpr :: [Int] -> String -> ExprV
makeExpr list name
    | cond == 0 = Leaf $ Constant 0
    | cond == 1 = terim (last list) len name
    | otherwise = BinaryOperation Plus (makeExpr newlist name) (terim (last list) len name)
    where cond = numofNonZero list
          len = (length list) - 1
          newlist = dropZeros (init list)

reducePoly :: ExprV -> ExprV
reducePoly x = makeExpr list name
    where name = if (length list) > 1 then func x else ""
          list = dropZeros (reduceHelper x)

-- an extra dummy variable, so as to not crash the GUI
notImpl :: ExprV
notImpl = Leaf $ Variable "Not Implemented"

