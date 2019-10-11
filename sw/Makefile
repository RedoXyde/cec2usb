# Target File and Directory
TARGET = Model
TARGET_DIR = .

# Source files, and Source directory
SRC =main.c hid.c
SRC_DIR = ./

# Installation Directory
INSTALL_DIR = .

# Run-time arguments
RUN_ARGS =

# Enable Debug Mode (compile with debug symbols, doesn't strip Target)
DEBUG = yes

# Compilation Flags
CFLAGS = -W -ansi -O2
# Linking Flags
LDFLAGS =  

ifeq ($(DEBUG),yes)
	CFLAGS += -g
	#LDFLAGS +=
else
	#CFLAGS +=
	LDFLAGS +=-s
endif

# Include Directories (/PATH/TO/DIR1 /DIR2 ...)
INCDIRS = /usr/include/
# List Extra Libs used (-lLIB1 -lLIB2)
LIBS = -lusb -lrt -lm

# Temporary Object files directory
OBJ_DIR = ./obj

# Tools
CC = gcc
CP = cp -v
RM = rm -vf
SHELL = sh

# Define Messages
MSG_ERRORS_NONE = Errors: none
MSG_START = ------------------------ Start ------------------------
MSG_END = ------------------------- End -------------------------
ifeq ($(DEBUG),yes)
	MSG_DEBUG = -------------------- DEBUG ENABLED --------------------
else
	MSG_DEBUG =
endif
MSG_LINK = Linking:
MSG_COMPILE = Compiling source:
MSG_CLEAN = Cleaning files:
MSG_RUN = Running $(TARGET):
MSG_INSTALL = Installing $(TARGET):

# Append Flags
# Add Curent dir as includes source dir
CFLAGS += -I.
# Add Includes Directories
CFLAGS +=$(patsubst %,-I%,$(INCDIRS))
# Define all object files.
OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)

# Default target. - Compile -
all:begin $(TARGET) end

# Run Target
# Launch the Target
run:
	@echo $(MSG_RUN)
	@echo
	@$(TARGET_DIR)/$(TARGET) $(RUN_ARGS)

# Clean
# Remove Target and Object Files
clean :
	@echo $(MSG_CLEAN)
	$(RM) $(TARGET_DIR)/$(TARGET) $(OBJ_DIR)/*.o

# Install
install :
	@echo $(MSG_INSTALL)
	$(CP) $(TARGET_DIR)/$(TARGET) $(INSTALL_DIR)

# Link
# Create output file from object files.
$(TARGET): $(OBJ)
	@echo $(MSG_LINK) $@
	$(CC) $(LIBS) $(LDFLAGS) $^ -o $(TARGET_DIR)/$(TARGET)
	@echo

# Compile
# Create object files from source files.
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	@echo $(MSG_COMPILE) $<
	$(CC) $(CFLAGS) -c $< -o $@
	@echo

# Eye-Candy
begin:
	@echo $(MSG_START)
	@echo $(MSG_DEBUG)

end:
	@echo $(MSG_DEBUG)
	@echo $(MSG_END)

# Show Version
version :
	@$(CC) --version

# Show Help
help:
	@echo "Available rules:"
	@echo " all ....... Compile project -Default rule-"
	@echo " version ... Show compiler version"
	@echo " clean ..... Erase Object files AND compiled project"
	@echo " run ....... Execute project (doesn't compile)"
	@echo " help ...... Show this help"
	@echo

# Create directories
$(shell mkdir $(OBJ_DIR) 2>/dev/null)
$(shell mkdir $(TARGET_DIR) 2>/dev/null)

# Phony Targets
.PHONY : all begin end version clean run help
