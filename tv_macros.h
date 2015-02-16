#ifndef TV_MACROS_H
#define TV_MACROS_H
/*****************************************************************************/
/*Error macros*/
#define tv_error(...) \
	fprintf(stderr, "Error: "); \
	fprintf(stderr, __VA_ARGS__); \
	fputc('\n', stderr); \
	exit(-1);
#define tv_warning(...) \
	fprintf(stderr, "Warning: "); \
	fprintf(stderr, __VA_ARGS__); \
	fputc('\n', stderr);
#define tv_info(...) \
	fprintf(stdout, "Info: "); \
	fprintf(stdout, __VA_ARGS__); \
	fputc('\n', stdout);

/*****************************************************************************/
/*Component macros                                                           */
#define require(required_component) \
	if(tv_component_get(self, require_component ## id() == NULL) { \
		tv_error("Required component of type \" ## required_component ## \" not attached."); \
	}

#define COMPONENT(component_prefix, parent_prefix) \
	tvuint component_prefix ## _id(); \
	void component_prefix ## _register(); \
	typedef struct component_prefix { \
		parent_prefix base;
#define ENDCOMPONENT(component_prefix) \
	} component_prefix; \
	component_prefix * component_prefix ## _new(); \
	void component_prefix ## _init(component_prefix *component);

#define COMPONENT_START(component_prefix) \
	static void start(tv_component *self_component) { \
		component_prefix *self = (component_prefix*)self_component;
#define END_COMPONENT_START \
	}
#define COMPONENT_UPDATE(component_prefix) \
	static void update(tv_component *self_component) { \
		component_prefix *self = (component_prefix*)self_component;
#define END_COMPONENT_UPDATE \
	}
#define COMPONENT_DESTROY(component_prefix) \
	static void destroy(tv_component *self_component) { \
		component_prefix *self = (component_prefix*)self_component;
#define END_COMPONENT_DESTROY \
		tv_free(self); \
	}

#define COMPONENT_NEW(component_prefix, parent_prefix) \
	static void start(tv_component *self); \
	static void update(tv_component *self); \
	static void destroy(tv_component *self); \
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


#define END_COMPONENT_NEW(component_prefix) \
		((tv_component*)self)->entity = NULL; \
		((tv_component*)self)->id = tv_cid_ ## component_prefix; \
		((tv_component*)self)->Start = start; \
		((tv_component*)self)->Update = update; \
		((tv_component*)self)->Destroy = destroy; \
		tv_component_notify_add((tv_component*)self); \
	}

/** 
 * Macro to call the destroy method of the given component.
 * Calls the "Destroy()" method.
 * CDESTROY(apple) --> apple->Destroy()
 * @param component the component to destroy.
 */
#define CDESTROY(component) (((tv_component*)component)->Destroy((tv_component*)component))

/** 
 * Macro to call the update method of the given component.
 * Calls the "Update()" method.
 * CUPDATE(apple) --> apple->Update()
 * @param component the component to update.
 */
#define CUPDATE(component) (((tv_component*)component)->Update((tv_component*)component))

/** 
 * Macro to call the start method of the given component.
 * Calls the "Start()" method. 
 * CSTART(apple) --> apple->Start()
 * @param component the component to start.
 */
#define CSTART(component) (((tv_component*)component)->Start((tv_component*)component))

/** 
 * Decalre a method for the given component.
 * Use this macro both for the declaration of methods (prototypes) as well as 
 * definitions (method bodies).
 * declare a method:
 *	void CMETHOD(a_component_type, a_method_name, int a, float b);
 * define a method:
 *	void CMETHOD(a_component_type, a_method_name, int a, float b) { ... }
 * @param component_prefix the component that this method operates on.
 * @param method_name the name of the method
 * @param ... the arguments to the method.
 */
#define CMETHOD(component_prefix, method_name, ...) component_prefix ## _ ## method_name(component_prefix *self, __VA_ARGS__)


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

#define TVCALL(METHOD, ...) METHOD (self, __VA_ARGS__ )

#endif
