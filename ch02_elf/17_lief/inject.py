import lief

libadd = lief.parse("libadd.so")
mul = lief.parse("mul")

add_symbol = libadd.get_symbol("add")
mul_symbol = mul.get_symbol("mul")

code_segment = mul.segment_from_virtual_address(mul_symbol.value)
segment_added = libadd.add(code_segment)
new_address = (
    segment_added.virtual_address + mul_symbol.value - code_segment.virtual_address
)
add_symbol.value = new_address
add_symbol.type = lief.ELF.SYMBOL_TYPES.FUNC
libadd.write("libadd_injected.so")
