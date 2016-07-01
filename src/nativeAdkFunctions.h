#include <stdlib.h>
#include <dynatrace_adk.h>

#define FILE "svnPostCommit.go"
#define API "Golang"

struct DTContext {
  int methodId;
  int serialNo;
};

// Initializes the DT Agent, should be called once at program startup
int initializeAgent();

// Terminates and shuts down the DT Agent, should be called once at program termination
void uninitializeAgent();

// Sensor placement with given method name and entry point indicator
// 0 for simple sensor placement, 1 for entry point sensor placement (start pure path)
struct DTContext placeSensor(char *methodName, int purePath);

// End point of the sensor, required after each place sensor
void exitSensor(struct DTContext ctx);

// Gets the DT tag as string
char* getTag();

// Inserts a synchronous link on the client side using the DT tag
void linkClientPurepath(char *tag);

// Captures a string parameter
void captureString(int serialNo, char *argument);

// Captures an int parameter
void captureInt(int serialNo, int argument);

// Captures a bool parameter
void captureBool(int serialNo, int argument);

// Returns a string value, marks end point of sensor
void returnStringAndExitSensor(struct DTContext ctx, char *argument);

// Returns an int value, marks end point of sensor
void returnIntAndExitSensor(struct DTContext ctx, int argument);

// Returns a bool value, marks end point of sensor
void returnBoolAndExitSensor(struct DTContext ctx, int argument);

// Logs the given log message, visible in PurePath
void logError(int methodId, char *message);
