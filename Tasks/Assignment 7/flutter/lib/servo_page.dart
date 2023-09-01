import 'main_page.dart';
import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:firebase_database/firebase_database.dart';

class ServoPage extends StatefulWidget {
  @override
  _ServoPageState createState() => _ServoPageState();
}

class _ServoPageState extends State<ServoPage> {
  int _ServoAngle = 0; // Initial angle

  // Function to handle changes in the slider value
  void _handleSliderChange(double newValue) {
    setState(() {
      _ServoAngle = newValue.toInt(); // Update servo angle state
    });

    // Update the value in the Firebase database
    DatabaseReference servoRef =
        FirebaseDatabase.instance.reference().child("Servo");
    servoRef.set(_ServoAngle);
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Servo Control'),
        backgroundColor: Colors.deepPurple[200],
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Slider(
              value: _ServoAngle.toDouble(), // Current servo angle value
              min: 0,
              max: 360,
              divisions: 4, // Divisions for 0, 90, 180, and 360 degrees
              label: '$_ServoAngle°', // Display the angle as label
              onChanged: _handleSliderChange, // Call the function on slider change
            ),
            Text(
              'Servo angle: $_ServoAngle°', // Show servo angle next to the slider
              style: TextStyle(fontSize: 18),
            ),
          ],
        ),
      ),
    );
  }
}