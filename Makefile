CC = g++
LINUXFLAGSDEBUG = -D _DEBUG -ggdb3 -O0 -std=c++17 -Wall -Wextra -Weffc++ \
	-Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations \
	-Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion \
	-Wctor-dtor-privacy -Wempty-body -Wformat-nonliteral -Wformat-security \
	-Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd \
	-Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow \
	-Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 \
	-Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand \
	-Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers \
	-Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector \
	-fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=580192 -Wstack-usage=8192 -pie -fPIE -Werror=vla \
	-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,$\
	nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,$\
	signed-integer-overflow,undefined,unreachable,vla-bound,vptr -mavx2

LINUXFLAGSRELIZE = -std=c++17 -Wall -mavx2 -O3

BIN_DIR = ./build/bin

OBJ_DIR = ./build/obj

$(BIN_DIR)/a.exe: $(OBJ_DIR)/main.o $(OBJ_DIR)/HashTable.o $(OBJ_DIR)/List.o $(OBJ_DIR)/FreadFile.o
	$(CC) $(OBJ_DIR)/main.o $(OBJ_DIR)/HashTable.o $(OBJ_DIR)/List.o $(OBJ_DIR)/FreadFile.o -o $(BIN_DIR)/a.exe $(LINUXFLAGSDEBUG)

$(OBJ_DIR)/main.o: main.cpp HashTable/HashTable.h
	@$(CC) -c main.cpp -o $(OBJ_DIR)/main.o $(LINUXFLAGSDEBUG)

$(OBJ_DIR)/HashTable.o: HashTable/HashTable.cpp HashTable/HashTable.h List/List.h
	@$(CC) -c HashTable/HashTable.cpp -o $(OBJ_DIR)/HashTable.o $(LINUXFLAGSDEBUG)

$(OBJ_DIR)/List.o: List/List.cpp List/List.h
	@$(CC) -c List/List.cpp -o $(OBJ_DIR)/List.o $(LINUXFLAGSDEBUG)

$(OBJ_DIR)/FreadFile.o: FreadFile/FreadFile.cpp FreadFile/FreadFile.h
	@$(CC) -c FreadFile/FreadFile.cpp -o $(OBJ_DIR)/FreadFile.o $(LINUXFLAGSDEBUG)

clean:
	rm -f main $(OBJ_DIR)/*.o
