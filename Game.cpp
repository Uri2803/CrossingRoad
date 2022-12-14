void Game::Level1()
{
	// line 1
	vehicleList.push_back(new Truck(10, 5, left_, 1));
	vehicleList.push_back(new Car(50, 6, left_, 1));
	animalList.push_back(new Deer(90, 5, left_, 1));

	// line 2
	vehicleList.push_back(new Car(10, 16, right_, 2));
	animalList.push_back(new Deer(50, 15, right_, 2));
	animalList.push_back(new Deer(90, 15, right_, 2));

	// u can random if u want
	lights.push_back(new TrafficLight(170, 15, 1000, 5000)); // (x, y, time for red light, time for green light ) count as millisecond

	// line 3 maybe 
	vehicleList.push_back(new Truck(10, 25, left_, 5));
	vehicleList.push_back(new Car(50, 26, left_, 2));
	vehicleList.push_back(new Car(90, 26, left_, 2));

	lights.push_back(new TrafficLight(5, 25, 2000, 3000)); // dont put 2 lights on 1 line with different timer :>, not funny

	// line 4
	vehicleList.push_back(new Truck(10, 35, right_, 4));
	vehicleList.push_back(new Car(50, 36, right_, 4));
	vehicleList.push_back(new Truck(90, 35, right_, 4));

	lights.push_back(new TrafficLight(170, 35, 3000, 4000));
}
void Game::Level2()
{
	// line 1
	vehicleList.push_back(new Truck(10, 5, left_, 1));
	vehicleList.push_back(new Car(50, 6, left_, 1));
	animalList.push_back(new Deer(90, 5, left_, 1));
	//line 2
	animalList.push_back(new Deer(50, 15, left_, 2));
	animalList.push_back(new Deer(90, 15, left_, 2));
	//line3
	vehicleList.push_back(new Truck(10, 25, right_, 5));
	vehicleList.push_back(new Car(50, 26, right_, 2));
	vehicleList.push_back(new Car(90, 26, right_, 2));

	//line4
	//testee
	//line5



}
void Game::Level3()
{

}
void Game::Level4()
{
	lights.push_back(new TrafficLight(170, 10, 500, 4000));
	lights.push_back(new TrafficLight(170, 35, 500, 4000));
	lights.push_back(new TrafficLight(5, 30, 1000, 3000));
	//line1
	vehicleList.push_back(new Truck(10, 5, left_, 3));
	vehicleList.push_back(new Car(50, 6, left_, 2));
	vehicleList.push_back(new Car(100, 6, left_, 2));
	//line2
	vehicleList.push_back(new Car(50, 11, right_, 3));
	vehicleList.push_back(new Truck(90, 10, right_, 2));
	vehicleList.push_back(new Truck(140, 10, right_, 2));
	//line3
	animalList.push_back(new Deer(50, 15, right_, 2));
	animalList.push_back(new Deer(90, 15, right_, 2));
	//line4
	animalList.push_back(new Deer(50, 20, left_, 3));
	animalList.push_back(new Deer(90, 20, left_, 3));
	animalList.push_back(new Deer(90, 20, right_, 2));
	//line5
	animalList.push_back(new Bird(90, 25, left_, 5));
	animalList.push_back(new Bird(100, 25, right_, 5));
	//line6
	vehicleList.push_back(new Truck(30, 30, left_, 2));
	vehicleList.push_back(new Car(60, 31, left_, 3));
	vehicleList.push_back(new Car(100, 31, left_, 2));
	//line7
	vehicleList.push_back(new Car(10, 36, right_, 2));
	vehicleList.push_back(new Car(50, 36, right_, 2));
	vehicleList.push_back(new Truck(140, 35, right_, 2));
	//line8
	vehicleList.push_back(new Car(20, 40, right_, 4));
	vehicleList.push_back(new Truck(110, 40, right_, 4));
}
