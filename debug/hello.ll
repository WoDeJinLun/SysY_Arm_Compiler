@.str = private unnamed_addr constant [ 8 x i8 ]c""sadas"\00", align 1
define i32 @main(){
%a = alloca i32 , align 4
%1 = ptrtoint i32*  %a to i64
%2 = sext i32 0 to i64
%3 = add i64 %1, %2
%4 = inttoptr i64 %3 to i32*
store i32 1000000, i32*  %4
%5 = call  @putf(i32*  @.str)
br label %LOOP_ENTRY_6
LOOP_ENTRY_6:
%6 = load i32,i32* %a
%7 = icmp sgt i32 %6, 0
%8 = zext i1 %7 to i32

%9 = icmp ne i32 %8, 0
br i1 %9, label %LOOP_BODY_7, label %LOOP_END_8
LOOP_BODY_7:
%10 = load i32,i32* %a
%11 = call  @putint(i32 %10)
%12 = call  @putch(i32 10)
%13 = load i32,i32* %a
%14 = sub i32 %13, 1
store i32 %14, i32*  %a
br label %LOOP_ENTRY_6
LOOP_END_8:
ret i32 0
}
