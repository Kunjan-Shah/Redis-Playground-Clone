# Redis Playground Clone

## Redis Playground
Redis is one the widely used caching service, particularly for distributed caching. At the most basic level, it is just a key value store. Redis playground is a command line interface for executing Redis commands online. You can explore the functionalities of Redis by playing around with the [Redis Playground](https://try.redis.io/).<br/>
In this project, I have created a clone of Redis playground CLI using C++.

## Commands supported
Compile the file using
```
g++ redis.cpp -o redis
```
Then, on the terminal run the following command to enter into the console
``` 
redis
```
Following commands are supported:
 * SET <key> <value><br/>
   * Stores the key-value pair in the KVStore if key does not already exist, otherwise updtes the value for the given key.
   * Returns "OK" on success
* GET <key><br/>
   * Finds the key in the KVStore
   * Returns value if key exists, otherwise returns "(nil)"
* EXISTS <key><br/>
   * Checks if key exists in the KVStore or not
   * Returns 1 if key exists, otherwise returns 0
* GETCACHESIZE
   * Returns current size of the cache
* PRINT
   * Prints the entire KVStore
* DEL
   * Deletes the key-value pair from the KVStore
   * Returns 1 if key existed and we actually deleted it, otherwise returns 0
