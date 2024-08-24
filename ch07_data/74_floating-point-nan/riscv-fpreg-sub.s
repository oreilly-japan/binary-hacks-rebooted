    .globl float_as_double
float_as_double:
    ret
    .globl as_float
as_float:
    fmv.s fa0, fa0
    ret
