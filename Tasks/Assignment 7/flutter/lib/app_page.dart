import 'package:flutter/material.dart';
import 'servo_page.dart'; // Import the servo.dart file
import 'graph.dart'; // Import the fetch.dart file

class ApplicationPage extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Welcome!'),
        backgroundColor: Colors.deepPurple[200],
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            ElevatedButton(
              onPressed: () {
                // Navigate to the FetchData page when "View Readings" button is pressed
                Navigator.push(
                  context,
                  MaterialPageRoute(builder: (context) => FetchData()),
                );
              },
              style: ElevatedButton.styleFrom(
                backgroundColor: Colors.deepPurple[200],
                minimumSize: Size(200, 60),
              ),
              child: Text(
                'View Readings',
                style: TextStyle(fontSize: 20),
              ),
            ),
            SizedBox(height: 16),
            ElevatedButton(
              onPressed: () {
                // Navigate to the ServoPage when "Control Servo" button is pressed
                Navigator.push(
                  context,
                  MaterialPageRoute(
                      builder: (context) =>
                          ServoPage()), // Navigate to ServoPage
                );
              },
              style: ElevatedButton.styleFrom(
                backgroundColor: Colors.blue[200],
                minimumSize: Size(200, 60),
              ),
              child: Text(
                'Control Servo',
                style: TextStyle(fontSize: 20),
              ),
            ),
          ],
        ),
      ),
    );
  }
}