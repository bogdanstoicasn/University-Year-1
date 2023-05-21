#### Copyright 2023-2024 Stoica Mihai-Bogdan 315CA (bogdanstoicasn@yahoo.com)
#### GNU GENERAL PUBLIC LICENSE 
#### Version 3, 29 June 2007

# **Load Balancer**

## Description

This is a README file that contains details and explanations
about the "Load Balancer" project.

# Specifics

The files contained in this project are as follows:

> hash.c, server.c, load_balancer.c, main.c

> hash.h, server.h, load_blalncer.h, utils.h

There may be constant definitions, such as **OOP** with a value of 64.

## Load Balancer

The topic addressed is the implementation of CONSISTEN HASHING. This is a
commonly used mechanism in distributed systems and it has the advantage of
meeting the minimal disruption constraint, which means minimizing the number
of transfers required when a server is stopped or started. Specifically, when
a server is stopped, only the objects located on that server need to be
redistributed to nearby servers. Similarly, when a new server is added, it will
only take over objects from a limited number of servers, the neighboring ones.

The key points of the project are the functions **from load_balancer.c** file:

> Allocating memory for the load balancer("**init_load_balancer**").

> Allocating memory for servers and puting the servers in the cyclic array
("**loader_add_server**").

> Removing a server("**loader_remove_server**").

> Introducing a product in the balancer("**loader_store**").

> Checking if a product is in a server("**loader_retrieve**").

> Free the allocated memory("**free_load_balancer**").

The only problems can be encountered when we remove a server(and it's copies) and
when we add a server(we have to reblance the items):
-Remove function:

> 1.get the coresponding indexes and the pointers to the said elements

> 2.removing the servers from the array

> 3.Redistribuing the elements on the remaining servers

-Add server function:

> 1.add new server and it's copies

> 2.taking pointer to products at coresponding servers

> 3.allocating new memory for hashtable

> 4.remaping the products again

This aproach to this function is not that good, because we use more memory , but
it helps recalculating the hashes of the items again. We free the items after the
remap.

## Resources

-> [Consistent Hashing](https://en.wikipedia.org/wiki/Consistent_hashing)

-> [Gaurav Sen](https://www.youtube.com/watch?v=zaRkONvyGr8&t=2s)

-> "The Joys of Hashing" by Thomas Mailund
