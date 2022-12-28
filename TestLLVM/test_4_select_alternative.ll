; ModuleID = 'WarpModule'
source_filename = "WarpModule"

@my_function_1_table = global [4 x i32 (i32)*] [i32 (i32)* @my_function_1_0, i32 (i32)* @my_function_1_1, i32 (i32)* @my_function_1_2, i32 (i32)* @my_function_1_3]

define i32 @my_function_1(i32 %parameter_2) {
entry:
  %0 = call i32 @my_function_1_select_index(i32 %parameter_2)
  %option_address = getelementptr [1 x i32 (i32)*], [1 x i32 (i32)*]* bitcast ([4 x i32 (i32)*]* @my_function_1_table to [1 x i32 (i32)*]*), i32 %0
  %cast_from_array = bitcast [1 x i32 (i32)*]* %option_address to i32 (i32)*
  %loaded_option = load i32 (i32)*, i32 (i32)* %cast_from_array, align 8
  %1 = call i32 %loaded_option(i32 %parameter_2)
  ret i32 %1
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
  %subtmp = sub i32 %my_parameter, 1
  %0 = call i32 @my_function_1(i32 %subtmp)
  %addtmp = add i32 %0, 0
  ret i32 %addtmp
}

define i32 @my_function_1_select_index(i32 %parameter_2) {
entry:
  %addtmp = add i32 0, %parameter_2
  %tmplt = icmp slt i32 %addtmp, 10
  %my_parameter_constrainttmp = select i1 true, i1 %tmplt, i1 false
  %index_matches = zext i1 %my_parameter_constrainttmp to i32
  %select_next_option = mul i32 %index_matches, 0
  %addtmp1 = add i32 0, %parameter_2
  %tmplt2 = icmp slt i32 %addtmp1, 42
  %addtmp3 = add i32 0, %parameter_2
  %tmpgeq = icmp sge i32 %addtmp3, 10
  %tmpand = select i1 %tmpgeq, i1 %tmplt2, i1 false
  %my_parameter_constrainttmp4 = select i1 true, i1 %tmpand, i1 false
  %index_matches5 = zext i1 %my_parameter_constrainttmp4 to i32
  %select_next_option6 = mul i32 %index_matches5, 1
  %0 = xor i1 %my_parameter_constrainttmp4, true
  %invert_index_matches = zext i1 %0 to i32
  %should_select_previous_option = mul i32 %invert_index_matches, %select_next_option
  %perform_selection = add i32 %select_next_option6, %should_select_previous_option
  %addtmp7 = add i32 0, %parameter_2
  %tmpgeq8 = icmp sge i32 %addtmp7, 42
  %my_parameter_constrainttmp9 = select i1 true, i1 %tmpgeq8, i1 false
  %index_matches10 = zext i1 %my_parameter_constrainttmp9 to i32
  %select_next_option11 = mul i32 %index_matches10, 2
  %1 = xor i1 %my_parameter_constrainttmp9, true
  %invert_index_matches12 = zext i1 %1 to i32
  %should_select_previous_option13 = mul i32 %invert_index_matches12, %perform_selection
  %perform_selection14 = add i32 %select_next_option11, %should_select_previous_option13
  %addtmp15 = add i32 0, %parameter_2
  %tmpgeq16 = icmp sge i32 %addtmp15, 1000
  %my_parameter_constrainttmp17 = select i1 true, i1 %tmpgeq16, i1 false
  %index_matches18 = zext i1 %my_parameter_constrainttmp17 to i32
  %select_next_option19 = mul i32 %index_matches18, 3
  %2 = xor i1 %my_parameter_constrainttmp17, true
  %invert_index_matches20 = zext i1 %2 to i32
  %should_select_previous_option21 = mul i32 %invert_index_matches20, %perform_selection14
  %perform_selection22 = add i32 %select_next_option19, %should_select_previous_option21
  ret i32 %perform_selection22
}

