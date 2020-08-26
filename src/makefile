include ../makefile.inc


OBJECTS = agent/Agent.o\
agent/AgentsCollection.o\
SimException.o\
agent/Antenna.o\
AntennaInfo.o\
CSVParser.o\
Clock.o\
Constants.o\
Displace.o\
RandomWalkDisplacement.o\
RandomWalkDriftDisplacement.o\
LevyFlightDisplacement.o\
AgeDistribution.o\
UniformAgeDistribution.o\
TruncatedNormalAgeDistribution.o\
PostLocProb.o\
NetPriorPostLocProb.o\
UnifPriorPostLocProb.o\
agent/MobileOperator.o\
EMField.o\
EventType.o\
map/Grid.o\
agent/HoldableAgent.o\
IDGenerator.o\
agent/ImmovableAgent.o\
InputParser.o\
agent/LocatableAgent.o\
map/Map.o\
map/WKTMap.o\
agent/MobilePhone.o\
agent/MovableAgent.o\
agent/Person.o\
PriorType.o\
RandomNumberGenerator.o\
agent/Tablet.o\
Tinyxml2.o\
Utils.o\
World.o\
main.o



$(TARGET): $(OBJECTS) 
	$(CXX) $(LDFLAGS) -o $@  $(OBJECTS)  $(LIBS)


install: $(TARGET)
	mkdir $(PROJ_HOME)/Release; mv $(TARGET) $(PROJ_HOME)/Release


.PHONY: clean
clean:
	rm -f *.o 
	rm -f *.d

.PHONY: cleanall
cleanall:
	rm -f *.o 
	rm -f *.exe
	rm -R $(PROJ_HOME)/Release/*
	rmdir $(PROJ_HOME)/Release

.PHONY: clbk
clbk:
	rm -f *~


.PHONY: dos2unix
dos2unix:
	dos2unix *

.PHONY: unix2dos
unix2dos:
	unix2dos *