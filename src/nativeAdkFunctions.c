#include <nativeAdkFunctions.h>

// Initializes the DT Agent, should be called once at program startup
int initializeAgent()
{
  int argCount = 1;
	char *strs[] = {"svnPostCommit-GoAgent"};
	char **argv = strs;
	//dynatrace_set("debugflags=debugTaggingADKNative=true,debugPurePathADKNative=true");
	dynatrace_set("agentname=GoCommitTrigger_CI_dynatrace");
	int res = dynatrace_initialize(&argCount, &argv);
	return res;
}

// Terminates and shuts down the DT Agent, should be called once at program termination
void uninitializeAgent() {
  dynatrace_uninitialize();
}

// Sensor placement with given method name and entry point indicator
// 0 for simple sensor placement, 1 for entry point sensor placement (start pure path)
struct DTContext placeSensor(char *methodName, int purePath) {
	int methodId = dynatrace_get_method_id(methodName, FILE, 0, API, 0);
	int serialNo = dynatrace_get_serial_no(methodId, purePath);
	serialNo = dynatrace_enter(methodId, serialNo);
  struct DTContext ctx = {methodId, serialNo};
  return ctx;
}

// End point of the sensor, required after each place sensor
void exitSensor(struct DTContext ctx) {
	dynatrace_exit(ctx.methodId, ctx.serialNo);
}

// Gets the DT tag as string
char* getTag() {
	char *tag = (char *)malloc(256 * sizeof(char));
	int retVal = dynatrace_get_tag_as_string(tag, 256 * sizeof(char));
	return (char *)tag;
}

// Inserts a synchronous link on the client side using the DT tag
void linkClientPurepath(char *tag) {
	dynatrace_link_client_purepath_by_string(1, tag);
}

// Captures a string parameter
void captureString(int serialNo, char *argument) {
  dynatrace_capture_string(serialNo, argument);
}

// Captures an int parameter
void captureInt(int serialNo, int argument) {
  dynatrace_capture_int32(serialNo, argument);
}

// Captures a bool parameter
void captureBool(int serialNo, int argument) {
  dynatrace_capture_bool(serialNo, !!(argument));
}

// Returns a string value, marks end point of sensor
void returnStringAndExitSensor(struct DTContext ctx, char *argument) {
  captureString(ctx.serialNo, argument);
  exitSensor(ctx);
}

// Returns an int value, marks end point of sensor
void returnIntAndExitSensor(struct DTContext ctx, int argument) {
  captureInt(ctx.serialNo, argument);
  exitSensor(ctx);
}

// Returns a bool value, marks end point of sensor
void returnBoolAndExitSensor(struct DTContext ctx, int argument) {
  captureBool(ctx.serialNo, argument);
  exitSensor(ctx);
}

// Logs the given log message, visible in PurePath
void logError(int methodId, char *message) {
  dynatrace_custom_log(methodId, message, 0, "GoErrorLogger");
}
