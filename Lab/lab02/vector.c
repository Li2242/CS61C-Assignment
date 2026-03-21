/* Include the system headers we need */
#include <stdlib.h>
#include <stdio.h>

/* Include our header */
#include "vector.h"

/* Define what our struct is */
struct vector_t {
    size_t size;
    int *data;
};

/* Utility function to handle allocation failures. In this
   case we print a message and exit. */
static void allocation_failed() {
    fprintf(stderr, "Out of memory.\n");
    exit(1);
}

/* Bad example of how to create a new vector */
vector_t *bad_vector_new() {
    /* Create the vector and a pointer to it */
    vector_t *retval, v; //在栈上分配了一个结构体,并且声明了一个指针变量,指针变量没有被初始化
    retval = &v;//返回栈上变量的地址,函数结束后这个地址就不再有效了

    /* Initialize attributes */
    retval->size = 1;
    retval->data = malloc(sizeof(int));
    if (retval->data == NULL) {
        allocation_failed();
    }

    retval->data[0] = 0;
    return retval;//返回指向栈上的指针
}

/* Another suboptimal way of creating a vector 栈分配*/
vector_t also_bad_vector_new() {
    /* Create the vector */
    vector_t v; //结构体在栈上

    /* Initialize attributes */
    v.size = 1;
    v.data = malloc(sizeof(int)); //数据在堆上
    if (v.data == NULL) {
        allocation_failed();
    }
    v.data[0] = 0;
    return v; //返回整个结构体副本,整个结构体都被复制了一边返回（副本还是在栈里面）
}

/* Create a new vector with a size (length) of 1
   and set its single component to zero... the
   RIGHT WAY 完全堆分配*/
vector_t *vector_new() {
    /* Declare what this function will return */
    vector_t *retval;

    /* First, we need to allocate memory on the heap for the struct */
    retval = (vector_t*)malloc(sizeof(vector_t));

    /* Check our return value to make sure we got memory */
    if (retval == NULL) {
        allocation_failed();
    }

    /* Now we need to initialize our data.
       Since retval->data should be able to dynamically grow,
       what do you need to do? */
    retval->size = 1;
    retval->data = (int*)malloc(sizeof(int));

    /* Check the data attribute of our vector to make sure we got memory */
    if (retval->data == NULL) {
        free(retval);				//Why is this line necessary?
        allocation_failed();
    }

    /* Complete the initialization by setting the single component to zero */
    retval->data[0] = 0;

    /* and return... */
    return retval; //仅返回一个指针-避免了数据结构的复制
}

/* Return the value at the specified location/component "loc" of the vector */
int vector_get(vector_t *v, size_t loc) {

    /* If we are passed a NULL pointer for our vector, complain about it and exit. */
    if(v == NULL) {
        fprintf(stderr, "vector_get: passed a NULL vector.\n");
        abort();
    }

    /* If the requested location is higher than we have allocated, return 0.
     * Otherwise, return what is in the passed location.
     */
    if (loc < v->size) {
        return v->data[loc];
    } else {
        return 0;
    }
}

/* Free up the memory allocated for the passed vector.
   Remember, you need to free up ALL the memory that was allocated. */
void vector_delete(vector_t *v) {
    free(v->data);
	free(v);
}

/* Set a value in the vector. If the extra memory allocation fails, call
   allocation_failed(). */
void vector_set(vector_t *v, size_t loc, int value) {
    /* What do you need to do if the location is greater than the size we have
     * allocated?  Remember that unset locations should contain a value of 0.
     */
	if(v == NULL){
		fprintf(stderr, "vector_set: passed a NULL vector.\n");
        abort();
	}
	if(loc < v->size){
		v->data[loc] = value;
	}else{
		v->data = realloc(v->data, sizeof(int)*(loc + 1));
		//检查分配是否成功
		if(v->data == NULL){
			allocation_failed();
		}
		//初始化刚分配的值(好习惯)
		for(size_t i = v->size; i < loc; i++){ //不等于0的原因是后面会给那个位置值不需要初始化
			v->data[i] = 0;
		}

		v->data[loc] = value;
		v->size = loc + 1; 
	}
}