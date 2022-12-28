; ModuleID = 'WarpModule'
source_filename = "WarpModule"

@my_function_1_table = global [4 x i32 (i32)*] [i32 (i32)* @my_function_1_0, i32 (i32)* @my_function_1_1, i32 (i32)* @my_function_1_2, i32 (i32)* @my_function_1_3]

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
  %tmpgeq6 = icmp sge i32 %addtmp5, 42
  %my_parameter_constrainttmp7 = select i1 true, i1 %tmpgeq6, i1 false
  %5 = zext i1 %my_parameter_constrainttmp7 to i32
  %6 = mul i32 %5, 2
  %7 = add i32 %4, %6
  %addtmp8 = add i32 0, %parameter_2
  %tmpgeq9 = icmp sge i32 %addtmp8, 1000
  %my_parameter_constrainttmp10 = select i1 true, i1 %tmpgeq9, i1 false
  %8 = zext i1 %my_parameter_constrainttmp10 to i32
  %9 = mul i32 %8, 3
  %10 = add i32 %7, %9
  %option_address = getelementptr [1 x i32 (i32)*], [1 x i32 (i32)*]* bitcast ([4 x i32 (i32)*]* @my_function_1_table to [1 x i32 (i32)*]*), i32 %10
  %cast_from_array = bitcast [1 x i32 (i32)*]* %option_address to i32 (i32)*
  %loaded_option = load i32 (i32)*, i32 (i32)* %cast_from_array, align 8
  %11 = call i32 %loaded_option(i32 %parameter_2)
  ret i32 %11
}

define i32 @my_function_1_0(i32 %my_parameter) {
entry:
  %multmp = mul i32 42, %my_parameter
  ret i32 %multmp
}

define i32 @my_function_1_1(i32 %my_parameter) {
entry:
  %addtmp = add i32 10, %my_parameter
  ret i32 %addtmp
}

define i32 @my_function_1_2(i32 %my_parameter) {
entry:
  %addtmp = add i32 0, %my_parameter
  %addtmp1 = add i32 %addtmp, %my_parameter
  ret i32 %addtmp1
}

define i32 @my_function_1_3(i32 %my_parameter) {
entry:
  %addtmp = add i32 %my_parameter, 1
  %0 = call i32 @my_function_1(i32 %addtmp)
  %addtmp1 = add i32 %0, 0
  ret i32 %addtmp1
}

