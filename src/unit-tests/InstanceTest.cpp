#include "gtest/gtest.h"
#include "Instance.h"

//Global handle to manager
Ptr<Instance::Manager> manager = shippingInstanceManager();


TEST(InstanceTest, managerCreation) {
  ASSERT_TRUE( NULL != manager );
}

TEST(InstanceTest, instanceNew) {
	Ptr<Instance> fleet = manager->instanceNew("myFleet", "Fleet");
	ASSERT_TRUE (NULL != fleet);
	Ptr<Instance> conn = manager->instanceNew("myConn", "Conn");
	ASSERT_TRUE (NULL != fleet);
	Ptr<Instance> stats = manager->instanceNew("myStats", "Stats");
	ASSERT_TRUE (NULL != fleet);
  Ptr<Instance> customer1 = manager->instanceNew("customer1", "Customer");  
	ASSERT_TRUE (NULL != customer1);  
  Ptr<Instance> port1 = manager->instanceNew("port1", "Port");
  ASSERT_TRUE (NULL != port1);
  Ptr<Instance> truckTerm1 = manager->instanceNew("truckTerm1", "Truck terminal");
  ASSERT_TRUE (NULL != truckTerm1);
  Ptr<Instance> boatTerm1 = manager->instanceNew("boatTerm1", "Boat terminal");
  ASSERT_TRUE (NULL != boatTerm1);
  Ptr<Instance> planeTerm1 = manager->instanceNew("planeTerm1", "Plane terminal");
  ASSERT_TRUE (NULL != planeTerm1);
  Ptr<Instance> truckSeg1 = manager->instanceNew("truckSeg1", "Truck segment");
  ASSERT_TRUE (NULL != truckSeg1);
  Ptr<Instance> boatSeg1 = manager->instanceNew("boatSeg1", "Boat segment");
  ASSERT_TRUE (NULL != boatSeg1);
  Ptr<Instance> planeSeg1 = manager->instanceNew("planeSeg1", "Plane segment");
  ASSERT_TRUE (NULL != planeSeg1);
  Ptr<Instance> boatSeg2 = manager->instanceNew("boatSeg2", "Boat segment"); 
  ASSERT_TRUE (NULL != boatSeg2); 
  Ptr<Instance> truckSeg2 = manager->instanceNew("truckSeg2", "Truck segment");
  ASSERT_TRUE (NULL != truckSeg2);
  Ptr<Instance> planeSeg2 = manager->instanceNew("planeSeg2", "Plane segment");
  ASSERT_TRUE (NULL != planeSeg2);
  Ptr<Instance> boatSeg3 = manager->instanceNew("boatSeg3", "Boat segment"); 
  ASSERT_TRUE (NULL != boatSeg3); 
  Ptr<Instance> truckSeg3 = manager->instanceNew("truckSeg3", "Truck segment");
  ASSERT_TRUE (NULL != truckSeg3);
  Ptr<Instance> planeSeg3 = manager->instanceNew("planeSeg3", "Plane segment");
  ASSERT_TRUE (NULL != planeSeg3);
}


TEST(InstanceTest, segments) { 
  
  
  Ptr<Instance> truckSeg1 = manager->instance("truckSeg1");
  Ptr<Instance> boatSeg1 = manager->instance("boatSeg1");
  Ptr<Instance> planeSeg1 = manager->instance("planeSeg1");
  Ptr<Instance> truckSeg2 = manager->instance("truckSeg2");
  Ptr<Instance> boatSeg2 = manager->instance("boatSeg2");
  Ptr<Instance> planeSeg2 = manager->instance("planeSeg2");
  Ptr<Instance> truckSeg3 = manager->instance("truckSeg3");
  Ptr<Instance> boatSeg3 = manager->instance("boatSeg3");
  Ptr<Instance> planeSeg3 = manager->instance("planeSeg3");
  
  truckSeg1->attributeIs("source", "customer1");
  truckSeg2->attributeIs("source", "port1");
  truckSeg3->attributeIs("source", "truckTerm1");
  truckSeg1->attributeIs("return segment", "truckSeg2");
  boatSeg1->attributeIs("source", "customer1");
  boatSeg2->attributeIs("source", "port1");
  boatSeg3->attributeIs("source", "boatTerm1");
  boatSeg1->attributeIs("return segment", "boatSeg2");
  planeSeg1->attributeIs("source", "customer1");
  planeSeg2->attributeIs("source", "port1");
  planeSeg3->attributeIs("source", "planeTerm1");
  planeSeg1->attributeIs("return segment", "planeSeg2");
  
  EXPECT_EQ("customer1", truckSeg1->attribute("source"));
  EXPECT_EQ("port1", truckSeg2->attribute("source"));
  EXPECT_EQ("truckTerm1", truckSeg3->attribute("source"));
  EXPECT_EQ("truckSeg2", truckSeg1->attribute("return segment"));
  EXPECT_EQ("truckSeg1", truckSeg2->attribute("return segment"));
  EXPECT_EQ("customer1", boatSeg1->attribute("source"));
  EXPECT_EQ("port1", boatSeg2->attribute("source"));
  EXPECT_EQ("boatTerm1", boatSeg3->attribute("source"));
  EXPECT_EQ("boatSeg2", boatSeg1->attribute("return segment"));
  EXPECT_EQ("boatSeg1", boatSeg2->attribute("return segment"));
  EXPECT_EQ("customer1", planeSeg1->attribute("source"));
  EXPECT_EQ("port1", planeSeg2->attribute("source"));
  EXPECT_EQ("planeTerm1", planeSeg3->attribute("source"));
  EXPECT_EQ("planeSeg2", planeSeg1->attribute("return segment"));
  EXPECT_EQ("planeSeg1", planeSeg2->attribute("return segment"));
}
