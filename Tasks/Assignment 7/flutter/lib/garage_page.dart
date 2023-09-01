import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:project_2/house_garage.dart';
//import 'package:google_fonts/google_fonts.dart';
import 'smart_device_box.dart';
import 'package:firebase_database/firebase_database.dart';

class GaragePage extends StatefulWidget {
  const GaragePage({super.key});

  @override
  State<GaragePage> createState() => _GaragePageState();
}

class _GaragePageState extends State<GaragePage> {
  // padding constants
  final double horizontalPadding = 40;
  final double verticalPadding = 25;

  // list of smart devices
  List mySmartDevices = [
    // [ smartDeviceName, iconPath , powerStatus ]
    ["Position one", "lib//icons/garage.png", true], 
    ["Position two", "lib/icons/garage.png", false],
    ["Position three", "lib/icons/garage.png", false],
    ["Position Four","lib/icons/garage.png", false],
  ];

  // power button switched
  void powerSwitchChanged(bool value, int index) {
    setState(() {
      mySmartDevices[index][2] = positions_values[index];
    });
  }

  final _database = FirebaseDatabase.instance.reference();
  List<String> positions_names = [];
  List<dynamic> positions_values = [];


  @override
  void initState(){
    super.initState();
    _activateListeners();
  }

  Future _activateListeners() async{
    _database.child('UsersData').child('Garage').onValue.listen((event) {
      Map<String,dynamic> description = event.snapshot.value! as Map<String,dynamic>;
      setState((){
        positions_names = description.keys.toList();
        positions_values = description.values.toList();
      });
      //print(positions_values);
    });
  }

  void car_button(int pos) async {
    final data = _database.child('UsersData').child('Robot');
    //final fan_status = database.child('UsersData').child('cx').child('readings').child('fan');
    
    try{
      await data.update({
        'Parking_Position' : pos
      });
    }
    catch(e){
      print('you got an error $e');
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Colors.deepPurple[200],
        leading: 
          GestureDetector(
            onTap: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(
                        builder: (context){
                          return HouseGarage();
                        }));
                        },
            child:Icon(Icons.arrow_back_ios_new),

          ),
          
        
      ),
      backgroundColor: Colors.grey[300],
      body: SafeArea(
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            // app bar
            Padding(
              padding: EdgeInsets.symmetric(
                horizontal: horizontalPadding,
                vertical: verticalPadding,
              ),
              child: Row(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: [
                  // menu icon
                  /*Image.asset(
                    'menu.png',
                    height: 45,
                    color: Colors.grey[800],
                  ),*/
                  Icon(Icons.menu),
                  // account icon
                  Icon(
                    Icons.person,
                    size: 45,
                    color: Colors.grey[800],
                  )
                ],
              ),
            ),

            const SizedBox(height: 20),


            Padding(
              padding: const EdgeInsets.symmetric(horizontal: 40.0),
              child: Center(
                child: Row(
                        mainAxisSize: MainAxisSize.min,
                        children: <Widget>[
                CupertinoButton(
                  color: Colors.deepPurple[200],
                  minSize: 5,
                  onPressed: !positions_values[0] 
                  ? () {
                    car_button(1);
                  }:null,
                  child: const Text('1'),
                ),
                const SizedBox(height: 10, width: 20,),
                CupertinoButton(
                  color: Colors.deepPurple[200],
                  minSize: 5,
                  onPressed: !positions_values[1] 
                  ? () {
                    car_button(2);
                  }:null,
                  child: const Text('2'),
                ),
                const SizedBox(height: 10,width:20),
                CupertinoButton(
                  color: Colors.deepPurple[200],
                  minSize: 5,
                  onPressed: !positions_values[2] 
                  ? () {
                    car_button(3);
                  }:null,
                  child: const Text('3'),
                ),
                const SizedBox(height: 10,width: 20,),
                CupertinoButton(
                  color: Colors.deepPurple[200],
                  minSize: 5,
                  onPressed: !positions_values[3] 
                  ? () {
                    car_button(4);
                  }:null,
                  child: const Text('4'),
                ),
                        ],
                      ),
              ),
            ),
            // welcome home
            /*Padding(
              padding: EdgeInsets.symmetric(horizontal: horizontalPadding),
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text(
                    "Welcome Home,",
                    style: TextStyle(fontSize: 20, color: Colors.grey.shade800),
                  ),
                  Text(
                    'Mitch Koko',
                    style: TextStyle(fontSize: 72)//GoogleFonts.bebasNeue(fontSize: 72),
                  ),
                ],
              ),
            ),*/

            const SizedBox(height: 25),

            const Padding(
              padding: EdgeInsets.symmetric(horizontal: 40.0),
              child: Divider(
                thickness: 1,
                color: Color.fromARGB(255, 204, 204, 204),
              ),
            ),

            const SizedBox(height: 25),

            // smart devices grid
            Padding(
              padding: EdgeInsets.symmetric(horizontal: horizontalPadding),
              child: Text(
                "Garage Positions",
                style: TextStyle(
                  fontWeight: FontWeight.bold,
                  fontSize: 24,
                  color: Colors.grey.shade800,
                ),
              ),
            ),
            const SizedBox(height: 10),

            // grid
            Expanded(
              child: GridView.builder(
                itemCount: 4,
                //physics: const NeverScrollableScrollPhysics(),
                padding: const EdgeInsets.symmetric(horizontal: 25,vertical: 5),
                gridDelegate: const SliverGridDelegateWithFixedCrossAxisCount(
                  crossAxisCount: 2,
                  childAspectRatio: 3.5 ,
                  //mainAxisSpacing : 10,
                  //crossAxisSpacing : 10,
                ),
                itemBuilder: (context, index) {
                  return SmartDeviceBox(
                    smartDeviceName: mySmartDevices[index][0],
                    iconPath: mySmartDevices[index][1],
                    powerOn: positions_values[index],
                    onChanged: null,
                  );
                },
              ),
              
            ),
          ],
        ),
        
      ),
    );
  }
}