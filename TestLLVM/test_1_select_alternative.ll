; ModuleID = 'WarpModule'
source_filename = "WarpModule"

declare i32 @print_integer(i32)

@my_function_1_table = global [3 x i32 (i32)*] [i32 (i32)* @my_function_1_0, i32 (i32)* @my_function_1_1, i32 (i32)* @my_function_1_2]

define i32 @my_function_1_0(i32 %my_parameter) {
entry:
  %ret = call i32 @print_integer(i32 0)
  %multmp = mul i32 42, %my_parameter
  ret i32 %multmp
}

define i32 @my_function_1_1(i32 %my_parameter) {
entry:
  %ret = call i32 @print_integer(i32 1)
  %addtmp = add i32 10, %my_parameter
  ret i32 %addtmp
}

define i32 @my_function_1_2(i32 %my_parameter) {
entry:
  %ret = call i32 @print_integer(i32 2)
  %addtmp = add i32 0, %my_parameter
  %addtmp1 = add i32 %addtmp, %my_parameter
  ret i32 %addtmp1
}

define i32 @my_function_1(i32 %parameter_2) {
entry:
  %addtmp = add i32 0, %parameter_2
  %tmplt = icmp slt i32 %addtmp, 10
  %my_parameter_constrainttmp = select i1 true, i1 %tmplt, i1 false
  %0 = zext i1 %my_parameter_constrainttmp to i32
  %1 = mul i32 %0, 0
  %addtmp1 = add i32 0, %parameter_2
  %tmplt2 = icmp slt i32 %addtmp1, 42
  %addtmp3 = add i32 0, %parameter_2
  %tmpgeq = icmp sge i32 %addtmp3, 10
  %tmpand = select i1 %tmpgeq, i1 %tmplt2, i1 false
  %my_parameter_constrainttmp4 = select i1 true, i1 %tmpand, i1 false
  %2 = zext i1 %my_parameter_constrainttmp4 to i32
  %3 = mul i32 %2, 1
  %4 = add i32 %1, %3
  %addtmp5 = add i32 0, %parameter_2
  %tmpgt = icmp sgt i32 %addtmp5, 42
  %my_parameter_constrainttmp6 = select i1 true, i1 %tmpgt, i1 false
  %5 = zext i1 %my_parameter_constrainttmp6 to i32
  %6 = mul i32 %5, 2
  %7 = add i32 %4, %6
  %option_address = getelementptr i32 (i32)*, [3 x i32 (i32)*]* @my_function_1_table, i32 %7
  %addr_as_int = ptrtoint i32 (i32)* %option_address to i32
  call i32 @print_integer(i32 %addr_as_int)
  %option = load ptr, [3 x i32 (i32)*]* %option_address, align 8
  %9 = call i32 %option(i32 %parameter_2)
  ret i32 %9
}
