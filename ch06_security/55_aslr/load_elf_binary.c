static int load_elf_binary(struct linux_binprm *bprm)
{
<!-- omit_middle -->
	if (!(current->personality & ADDR_NO_RANDOMIZE) && randomize_va_space)
		current->flags |= PF_RANDOMIZE;

	setup_new_exec(bprm); ― ①

	/* Do this so that we can load the interpreter, if need be.  We will
	   change some of these later */
	retval = setup_arg_pages(bprm, randomize_stack_top(STACK_TOP), ― ②
				 executable_stack);
<!-- omit_middle -->
	for(i = 0, elf_ppnt = elf_phdata;
	    i < elf_ex->e_phnum; i++, elf_ppnt++) {
<!-- omit_middle -->
		} else if (elf_ex->e_type == ET_DYN) {
<!-- omit_middle -->
			if (interpreter) {
				load_bias = ELF_ET_DYN_BASE;
				if (current->flags & PF_RANDOMIZE)
					load_bias += arch_mmap_rnd(); ― ③
				alignment = maximum_alignment(elf_phdata, elf_ex->e_phnum);
				if (alignment)
					load_bias &= ~(alignment - 1);
				elf_flags |= MAP_FIXED_NOREPLACE;
			} else
				load_bias = 0;
<!-- omit_middle -->
    }

		error = elf_map(bprm->file, load_bias + vaddr, elf_ppnt, ― ④
				elf_prot, elf_flags, total_size);
<!-- omit_middle -->
	}
<!-- omit_middle -->
	if ((current->flags & PF_RANDOMIZE) && (randomize_va_space > 1)) {
<!-- omit_middle -->
		mm->brk = mm->start_brk = arch_randomize_brk(mm); ― ⑤
<!-- omit_end -->
