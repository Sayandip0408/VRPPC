
# Vehicle Routing Problem with Private fleet and Common carrier

The vehicle routing problem (VRP) is a combinatorial optimization and integer programming problem which asks "What is the optimal set of routes for a fleet of vehicles to traverse in order to deliver to a given set of customers?" It generalises the travelling salesman problem (TSP). It first appeared in a paper by George Dantzig and John Ramser in 1959, in which the first algorithmic approach was written and was applied to petrol deliveries. Often, the context is that of delivering goods located at a central depot to customers who have placed orders for such goods. The objective of the VRP is to minimize the total route cost. In 1964, Clarke and Wright improved on Dantzig and Ramser's approach using an effective greedy algorithm called the savings algorithm.

The vehicle-routing problem with private fleet and common carrier (VRPPC) is a variant of the VRP in which customers can be subcontracted at a customer-dependent cost if the privately-owned capacity is insufficient to serve all customers, or if doing so is beneficial from a cost point of view. Consequently, the subcontracted customers do not need to be served on vehicle routes of the privately-owned fleet, but a cost is paid for outsourcing customers to the so-called common carrier.

# Data Reading and preprocessing

To solve this problem we were given text files, which is consists of
Name of the Text Dataset, some comments, Number of customers,
Number of vehicles, Details of every customer (i.e. Customer Number,
Coordinates, Demands, External Cost) and Details of every vehicle (i.e.
Truck ID, Capacity, Fixed Cost, Variable Cost).

First thing we had to do is reading the dataset using a function named
takeInputFile(). Then we’re reading the first five line of every dataset.
Line number 3 and 4 was important for us because it holds the value
of total customers and total vehicles respectively.

Declared two integer variables named customer and vehicle, storing
the number of customers in customer and number of customers in
vehicle. Based on customer and vehicle we’re going to read next lines
of the dataset and storing the data in respective arrays.

customerArr is declared to store the details of customers, the size of
this array is [(customer + 1) * 5]. Then all the customer details are read
and stored in the customerArr using getCustomerData() function.

Similarly, vehicleArr is declared to store the details of vehicles, the size
of this array is [vehicle * 4]. Then all the vehicle details are read and
stored in the vehicleArr using getVehicleData() function.

When reading of data is completed and stored in respective arrays,
next part is to create a distance matrix of customer locations (using
coordinates) using getDistanceMatrix() function which is a 2D vector.

# Objective Function

This is a function for computing the objective value after obtaining the routes.

The calculations are as follows :

i. Variable cost of a vehicle (A) = total distance traveled * VAR. COST

ii. Total cost for the internal fleet (B) = FIXED COST for the used
vehicles + variable cost for the used vehicles (A)

iii. Objective value = B + Cost of external transporter for the external
vehicles

# To read more please check the VRPPC_documentationNew.pdf
