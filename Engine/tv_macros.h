#ifndef TV_MACROS_H
#define TV_MACROS_H
/*****************************************************************************/
/*Error macros*/
/** 
 * Generates an error message and terminates the engine.
 * @param ... the message to print (in printf formatted string format).
 */
#define tv_error(...) \
	fprintf(stderr, "Error: "); \
	fprintf(stderr, __VA_ARGS__); \
	fputc('\n', stderr); \
	exit(-1);
/**
 * Generates a warning message.
 * @param ... the formatted string message to print.
 */
#define tv_warning(...) \
	fprintf(stderr, "Warning: "); \
	fprintf(stderr, __VA_ARGS__); \
	fputc('\n', stderr);
/**
 * Generates an informational message.
 * @param ... the formatted string message to print.
 */
#define tv_info(...) \
	fprintf(stdout, "Info: "); \
	fprintf(stdout, __VA_ARGS__); \
	fputc('\n', stdout);
/*****************************************************************************/
/*Definition Macros                                                          */
/** 
 * Begins a start definition for a component.
 * @param component_prefix the component to define the "start" method for.
 */
#define COMPONENT_START(component_prefix) \
	static void component_prefix ## _start(tv_component *self_component) { \
		component_prefix *self = (component_prefix*)self_component;
/** 
 * Concludes a start definition for a component.
 * @param component_prefix the component to end the "start" method definition.
 */
#define END_COMPONENT_START \
	}
/** 
 * Begins an update definition for a component.
 * @param component_prefix the component to define the "update" method for.
 */
#define COMPONENT_UPDATE(component_prefix) \
	static void component_prefix ## _update(tv_component *self_component) { \
		component_prefix *self = (component_prefix*)self_component;
/** 
 * Concludes an update definition for a component.
 * @param component_prefix the component to end the "update" method definition.
 */
#define END_COMPONENT_UPDATE \
	}
/** 
 * Begins a destroy definition for a component.
 * @param component_prefix the component to define the "destroy" method for.
 */
#define COMPONENT_DESTROY(component_prefix) \
	static void component_prefix ## _destroy(tv_component *self_component) { \
		component_prefix *self = (component_prefix*)self_component;
/** 
 * Concludes a destroy definition for a component.
 * @param component_prefix the component to end the "destroy" method definition.
 */
#define END_COMPONENT_DESTROY \
		tv_free(self); \
	}
/** 
 * Begins a "new" definition for a component.
 * @param component_prefix the component to define the "new" method for.
 * @param parent_prefix the parent component of the component - the parent's
 *	initializer (NEW) method is called prior to initialization of this 
 *  component.
 */
#define COMPONENT_NEW(component_prefix, parent_prefix) \
	static void component_prefix ##_start(tv_component *self); \
	static void component_prefix ## _update(tv_component *self); \
	static void component_prefix ## _destroy(tv_component *self); \
	static tvuint tv_cid_ ## component_prefix = 0; \
	tvuint component_prefix ## _id() { \
		return tv_cid_ ## component_prefix; \
	} \
	void component_prefix ## _register() { \
		if(!(parent_prefix ## _id())) { \
			parent_prefix ## _register(); \
		} \
		if(!tv_cid_ ## component_prefix) { \
			tv_component_register_id(&tv_cid_ ## component_prefix, parent_prefix ## _id(), sizeof(component_prefix)); \
		} \
	} \
	component_prefix * component_prefix ## _new() { \
		component_prefix *self = (component_prefix *)tv_alloc(sizeof(component_prefix)); \
		component_prefix ## _init(self); \
		return self; \
	} \
	void component_prefix ## _init(component_prefix * self) {
/** 
 * Concludes a "new" definition for a component.
 * @param component_prefix the component to end the "new" method definition for.
 */
#define END_COMPONENT_NEW(component_prefix) \
		((tv_component*)self)->entity = NULL; \
		((tv_component*)self)->id = tv_cid_ ## component_prefix; \
		((tv_component*)self)->Start = component_prefix ## _start; \
		((tv_component*)self)->Update = component_prefix ## _update; \
		((tv_component*)self)->Destroy = component_prefix ## _destroy; \
		tv_component_notify_add((tv_component*)self); \
	}
/** 
 * Declare/define a method for the given component.
 * Use this macro both for the declaration of methods (prototypes) as well as 
 * definitions (method bodies).
 * declare a method:
 *	void METHOD(a_component_type, a_method_name, int a, float b);
 * define a method:
 *	void METHOD(a_component_type, a_method_name, int a, float b) { ... }
 * @param component_prefix the component that this method operates on.
 * @param method_name the name of the method
 * @param ... the arguments to the method.
 */
#define METHOD(component_prefix, method_name, ...) component_prefix ## _ ## method_name(component_prefix *self, __VA_ARGS__)

/*****************************************************************************/
/*Declaration Macros                                                         */
/**
 * Declares a component of the given name descended from the component of the
 * given parent component.
 * @param component_prefix the name of the component - used as prefix for all
 *	methods related to this component.
 * @param parent_prefix the name of the parent of this component - e.g. a 
 *	tv_widget_textbox is descended from "tv_widget".
 */
#define COMPONENT(component_prefix, parent_prefix) \
	tvuint component_prefix ## _id(); \
	void component_prefix ## _register(); \
	typedef struct component_prefix { \
		parent_prefix base;
/**
 * Concludes the declaration of a component of the given name.
 * @param component_prefix the name of the component whose declaration is to
 *	be closed.
 */
#define ENDCOMPONENT(component_prefix) \
	} component_prefix; \
	component_prefix * component_prefix ## _new(); \
	void component_prefix ## _init(component_prefix *component);
/**
 * Use this macro in a component's header file to declare a member variable of 
 * that component as public.
 * Public variables may be accessed in the inspector (if they are registered 
 * with CACCESSOR), and they have get and set methods that may be used
 * to view and alter the value of the variable by other components.
 * @param component_prefix the component that the variable belongs to.
 * @param variable the name of the variable to declare public.
 */
#define CPUBLIC(component_prefix, type, variable) \
	static type component_prefix ## _get(component_prefix *c) {return c-> variable );} \
	static void component_prefix ## _set(component_prefix *c, type new_val) {c-> variable = new_val;} \
	static void component_prefix ## _make_public (const tvchar* component_name, const tvchar* variable_name, tvuint cid) \
		{ \
			type (*_get)( component_prefix *) = component_prefix ## _get;\
			void (*_set)( component_prefix *, type ) = component_prefix ## _set; \
			tv_register_public_variable( #component_prefix , #variable , #type, name, _get, _set); \
		}

/*****************************************************************************/
/*Component Function Macros                                                  */
#define require(required_component) \
	if(tv_component_get(self, require_component ## id() == NULL) { \
		tv_error("Required component of type \" ## required_component ## \" not attached."); \
	}
/**
 * Creates a component of the given name.
 * @param component_name the name of the component to create e.g. "tv_model".
 */
#define NEW(component_name) (( component_name *)component_name ## _new())
/** 
 * Macro to call the destroy method of the given component.
 * Calls the "Destroy()" method.
 * DESTROY(apple) --> apple->Destroy()
 * @param component the component to destroy.
 */
#define DESTROY(component) (((tv_component*)component)->Destroy((tv_component*)component))
/** 
 * Macro to call the update method of the given component.
 * Calls the "Update()" method.
 * UPDATE(apple) --> apple->Update()
 * @param component the component to update.
 */
#define UPDATE(component) (((tv_component*)component)->Update((tv_component*)component))
/** 
 * Macro to call the start method of the given component.
 * Calls the "Start()" method. 
 * START(apple) --> apple->Start()
 * @param component the component to start.
 */
#define START(component) (((tv_component*)component)->Start((tv_component*)component))
/**
 * Store a component of the specified type in the given component member
 * attached to.
 * @param component_ref the member of "self" to store the component in.
 * @param component_to_get the component type to get e.g. "tv_model"
 */
#define GET(component_ref, component_to_get) self-> ## component_ref = ( component_to_get *)(tv_component_get((tv_component*)self, component_to_get ## _id()))
/**
 * Call the given method of the given component.
 * @param component_name the name of the component e.g. "tv_model"
 * @param method_name the name of the method e.g. "optimize"
 */
#define CALL(component_name, method_name, ...) component_name ## _ ## method_name (self, __VA_ARGS__ ) 
/**
 * Put this in the COMPONENT_NEW method to allow access to a variable by the
 * inspector.
 * @param component_prefix the prefix of the component to add.
 * @param variable the name of the variable to add.
 */
#define CACCESSOR(component_prefix, variable) \
	component_prefix ## _make_public(#component_prefix , #variable , component_prefix_id())

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*Handler macros 														     */
#define HANDLER(handler_name, parent_name) \
	tvuint handler_name ## _id(); \
	void handler_name ## _register(); \
	typedef struct { \
		parent_name base;
#define ENDHANDLER(handler_name) \
	} handler_name; \
	handler_name * handler_name ## _new(); 

#define HANDLER_START(handler_name) \
	static void start(tv_component *self_component) { \
		handler_name *self = (handler_name*)self_component;
#define END_HANDLER_START \
	}

#define HANDLER_UPDATE(handler_name) \
	static void update(tv_component *self_component) { \
		handler_name *self = (handler_name*)self_component;
#define END_HANDLER_UPDATE \
	}

#define HANDLER_NEW(handler_name, parent_name, handler_func, stage) \
	static void start(tv_component *self); \
	static void update(tv_component *self); \
	static tvuint tv_cid_ ## handler_name = 0; \
	static void handler_func(tv_component *self); \
	tvuint handler_name ## _id() { \
		return tv_cid_ ## handler_name; \
	} \
	void handler_name ## _register() { \
		if(!(parent_name ## _id())) { \
			parent_name ## _register(); \
		} \
		if(!tv_cid_ ## handler_name) { \
		tv_component_register_handler(&tv_cid_ ## handler_name, parent_name ## _id(), handler_func, stage, sizeof(handler_name)); \
		} \
	} \
	handler_name * handler_name ## _new() { \
		handler_name *self = (handler_name *)tv_alloc(sizeof(handler_name)); 
#define END_HANDLER_NEW(handler_name) \
		((tv_component*)self)->entity = NULL; \
		((tv_component*)self)->id = tv_cid_ ## handler_name; \
		tv_component_register_to_handler(((tv_component*)self)->id, (tv_component*)self); \
		((tv_component*)self)->Start = start; \
		((tv_component*)self)->Update = update; \
		((tv_component*)self)->Destroy = NULL; \
		return self; \
	}

#endif
