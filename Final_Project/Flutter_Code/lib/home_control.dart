import 'dart:async';

import 'package:firebase_auth/firebase_auth.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';
import 'package:project_2/house_garage.dart';
import 'package:project_2/house_options_page.dart';
//import 'package:google_fonts/google_fonts.dart';
import 'smart_device_box.dart';

class HomeControlPage extends StatefulWidget {
  const HomeControlPage({super.key});

  @override
  State<HomeControlPage> createState() => _HomeControlPageState();
}

class _HomeControlPageState extends State<HomeControlPage> {
  // padding constants
  final double horizontalPadding = 40;
  final double verticalPadding = 25;
  final database = FirebaseDatabase.instance.reference();



  // list of smart devices
  List mySmartDevices = [
    // [ smartDeviceName, iconPath , powerStatus ]
    ["Light", "lib//icons//light-bulb.png", false], 
    ["Fan","lib/icons/fan.png", false],
  ];

  //List status = [];

  // power button switched
  void powerSwitchChanged(bool value, int index) async {
    final data = database.child('UsersData').child('Home');
    //final fan_status = database.child('UsersData').child('cx').child('readings').child('fan');
    
    setState(() {
      mySmartDevices[index][2] = value;
    });

    try{
      await data.update({
        mySmartDevices[index][0] : mySmartDevices[index][2],
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
        title: Text('House Control',
        style: TextStyle(fontSize: 16),
        ),
        centerTitle: true,
        backgroundColor: Colors.deepPurple[200],
        leading: 
          GestureDetector(
            onTap: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(
                        builder: (context){
                          return HouseOptions();
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
                "House Control",
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
                itemCount: mySmartDevices.length,
                //physics: const NeverScrollableScrollPhysics(),
                padding: const EdgeInsets.symmetric(horizontal: 25),
                gridDelegate: const SliverGridDelegateWithFixedCrossAxisCount(
                  crossAxisCount: 2,
                  childAspectRatio: 2.5,
                ),
                itemBuilder: (context, index) {
                  return SmartDeviceBox(
                    smartDeviceName: mySmartDevices[index][0],
                    iconPath: mySmartDevices[index][1],
                    powerOn: mySmartDevices[index][2],
                    onChanged: (value) => powerSwitchChanged(value, index),
                  );
                  
                },
              ),
            )
          ],
        ),
      ),
    );
  }
}