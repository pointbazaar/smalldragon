#this build script
#tests that all examples transpile

#the tests are in the same order as a 'tree examples/' would show them

CALL  := sd -Werror
CALL2 := sd -Werror ../stdlib/intstack.dg ../stdlib/ppm.dg ../stdlib/math.dg

all: test

test: 
	@echo "[Examples] Transpiling Example Programs..."
	$(CALL) booleans/Booleans.dg
	$(CALL) break/Break.dg
	$(CALL) chars/Chars.dg
	$(CALL) cmd-args/CmdArgs.dg
	$(CALL) floats/Floats.dg
	$(CALL) functional/Filter.dg 
	$(CALL) functional/Twice.dg
	$(CALL) ifstatement/IfStatement.dg 
	$(CALL) input/Input.dg 
	$(CALL) local_variables/Locals.dg 
	$(CALL) local_variables/Locals2.dg 
	$(CALL) loops/WhileLoop.dg 
	$(CALL) loops/ForLoop.dg 
	$(CALL) loops/Loop.dg 
	$(CALL) mathematics/Faculty.dg 
	$(CALL) mathematics/Fibonacci.dg 
	$(CALL) methodCalls/MethodCall.dg 
	$(CALL) other/Everything.dg 
	$(CALL) other/Harshad.dg 
	$(CALL) structs/MyStructs.dg 
	$(CALL) structs/Node.dg 
	$(CALL) switch/switch.dg 
	$(CALL) file_io/file_io.dg
	$(CALL) plotting/plotting.dg
	$(CALL) lambda.dg
	$(CALL2) annotations.dg
	$(CALL) typeinference/LocalVarTypeInference.dg

	$(CALL) try.dg
	$(CALL) try2.dg

	$(CALL2) image-io.dg
	@echo "[Examples] done"

# currently not working: 
# - generics/genericstruct.dg

clean:
	rm -f */*.tokens
	rm -f */*.ast
	rm -f */*.c
	rm -f */*.out
