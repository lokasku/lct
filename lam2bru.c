/*
 * Convert normal lambda calculus to De Bruijn notation
 *
 * (\ 0 0) -> (\x. x x)
 *  (\\ 0 1) -> (\x.\y. y x)
 */
