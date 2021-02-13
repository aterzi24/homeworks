def is_firmus(a,b):
    eps = 0.0009
    if min( a[1] , a[3] ) > min( b[1] , b[3] ):
        a , b = b , a
    a = [ min( a[0] , a[2] ) , min( a[1] , a[3] ) , max( a[0] , a[2] ) , max( a[1] , a[3] ) ]
    b = [ min( b[0] , b[2] ) , min( b[1] , b[3] ) , max( b[0] , b[2] ) , max( b[1] , b[3] ) ]
    area = ( float( a[2] ) - a[0] ) * ( a[3] - a[1] ) + ( b[2] - b[0] ) * ( b[3] - b[1] )
    k = min( a[2] , b[2] ) - max( a[0] , b[0] )
    if k < 0 or b[1] > a[3] + eps or a[1] and abs( b[1] - a[3] ) < eps:
        return [ "DAMNARE" , area ]
    if b[1] + eps < a[3]:
        return [ "DAMNARE" , area - k * ( min( a[3] , b[3] ) - b[1] ) ]
    if eps < ( b[0] + b[2] ) / 2.0 - a[2]:
        return [ "ADDENDUM" , [ 2 * a[2] - b[2] , b[1] , b[0] , b[3] ] ]
    if eps < a[0] - ( b[0] + b[2] ) / 2.0:
        return [ "ADDENDUM" , [ b[2] , b[1] , 2 * a[0] - b[0] , b[3] ] ]
    return [ "FIRMUS" , area ]


print is_firmus([0,0,4,4],[4,4,8,8])
