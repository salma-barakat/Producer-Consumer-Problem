# Producer-Consumer-Problem
Implementing the Bounded-Buffer Producer/Consumer problem,
each Producer would declare the live price of a commodity (e.g., GOLD, SILVER, CRUDEOIL, COTTON, …). One Consumer would show a dashboard for the prices of all commodities. Producers and Consumer would run indefinitely sharing the prices through shared memory. <br />
•The command “./producer NATURALGAS 7.1 0.5 200” would run a producer that declares the current price of Natural Gas every 200ms according to a normal distribution with parameters (mean=0.5 and variance=0.25). <br />
•While running the consumer, you will specify the argument N – an integer which indicates the Bounded-Buffer Size.
