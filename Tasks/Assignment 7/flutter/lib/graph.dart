import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';
import 'package:fl_chart/fl_chart.dart';
import 'package:project_2/app_page.dart';
import 'package:project_2/house_options_page.dart';
import 'package:project_2/login_page.dart';
import 'package:project_2/servo_page.dart';


class FetchData extends StatefulWidget {
  const FetchData({Key? key}) : super(key: key);

  @override
  State<FetchData> createState() => _FetchDataState();
}

class _FetchDataState extends State<FetchData> {
  late DatabaseReference reference;

  double sensor = 0;
  double distance = 0;

  final sensorController = TextEditingController();
  final distanceController = TextEditingController();

  bool isEditing = false;
  late String currentReadingKey;

  List<double> sensorData = [];

  @override
  void initState() {
    super.initState();
    // Set up Firebase reference
    reference = FirebaseDatabase.instance
        .reference()
        .child('UsersData')
        .child('readings');

    // Listen for child added events on the reference
    reference.orderByKey().onChildAdded.listen((event) {
      if (event.snapshot.value != null) {
        currentReadingKey = event.snapshot.key!;
        Map<dynamic, dynamic>? snapshotValue =
            event.snapshot.value as Map<dynamic, dynamic>?;

        if (snapshotValue != null) {
          setState(() {
            // Update sensor and distance values
            sensor = (snapshotValue['irsensor'] as num).toDouble();
            sensorData.add(sensor);
            sensorController.text = sensor.toString();

            distance = (snapshotValue['Distance'] as num).toDouble();
            distanceController.text = distance.toString();

            // Show a pop-up if distance is critical
            if (distance < 3) {
              _showCriticalDistanceSnackBar();
            }
          });
        }
      }
    });
  }

  void updateReading() {
    // Get new sensor and distance values from controllers or use current values if parsing fails
    double newIrsensor = double.tryParse(sensorController.text) ?? sensor;
    double newDistance = double.tryParse(distanceController.text) ?? distance;

    // Update values in Firebase
    reference.child(currentReadingKey).update({
      'irsensor': newIrsensor,
      'Distance': newDistance,
    });

    // Clear controllers and reset editing state
    sensorController.clear();
    distanceController.clear();

    setState(() {
      isEditing = false;
    });

    // Show pop-up if distance is critical after update
    if (newDistance < 3) {
      _showCriticalDistanceSnackBar();
    }
  }

  void _showCriticalDistanceSnackBar() {
    // Show a snackbar with options to open or not open a door
    ScaffoldMessenger.of(context).showSnackBar(
      SnackBar(
        backgroundColor: Colors.green,
        content: Row(
          mainAxisAlignment: MainAxisAlignment.spaceBetween,
          children: [
            Text(
              'Critical distance',
              style: TextStyle(color: Colors.white),
            ),
            ElevatedButton(
              onPressed: () {
                // Navigate to the ServoPage when button is pressed
                Navigator.push(
                  context,
                  MaterialPageRoute(builder: (context) => ServoPage()),
                );
              },
              child: Text('Open door'),
            ),
            ElevatedButton(
              onPressed: () {
                // Navigate to the AppPage when button is pressed
                Navigator.push(
                  context,
                  MaterialPageRoute(builder: (context) => ApplicationPage()),
                );
              },
              child: Text("Don't open door"),
            ),
          ],
        ),
      ),
    );
  }

  @override
  void dispose() {
    // Dispose of controllers when the widget is disposed
    sensorController.dispose();
    distanceController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(
          'Reading Editor',
          style: TextStyle(
            fontSize: 26,
            fontWeight: FontWeight.w500,
          ),
        ),
        centerTitle: true,
        backgroundColor: Colors.deepPurple[200],
      ),
      body: SingleChildScrollView(
        child: Column(
          children: [
            SizedBox(
              height: 300,
              child: Center(
                child: LineChart(
                  LineChartData(
                    lineBarsData: [
                      LineChartBarData(
                        spots: List.generate(
                            sensorData.length,
                            (index) =>
                                FlSpot(index.toDouble(), sensorData[index])),
                        isCurved: true,
                        colors: [Colors.blue],
                        dotData: FlDotData(show: false),
                      ),
                    ],
                    titlesData:
                        FlTitlesData(leftTitles: SideTitles(showTitles: true)),
                  ),
                ),
              ),
            ),
            Center(
              child: Container(
                margin: const EdgeInsets.all(10),
                padding: const EdgeInsets.all(10),
                color: Color.fromARGB(255, 231, 160, 218),
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    Row(
                      mainAxisAlignment: MainAxisAlignment.spaceBetween,
                      children: [
                        Text(
                          'irsensor: ',
                          style: TextStyle(
                              fontSize: 14, fontWeight: FontWeight.w400),
                        ),
                        isEditing
                            ? SizedBox(
                                width: 120,
                                child: TextField(
                                  controller: sensorController,
                                  keyboardType: TextInputType.number,
                                  decoration: InputDecoration(
                                    border: OutlineInputBorder(),
                                  ),
                                ),
                              )
                            : Text(
                                '$sensor',
                                style: TextStyle(
                                    fontSize: 14, fontWeight: FontWeight.w400),
                              ),
                      ],
                    ),
                    const SizedBox(
                      height: 10,
                    ),
                    Row(
                      mainAxisAlignment: MainAxisAlignment.spaceBetween,
                      children: [
                        Text(
                          'Distance: ',
                          style: TextStyle(
                              fontSize: 14, fontWeight: FontWeight.w400),
                        ),
                        isEditing
                            ? SizedBox(
                                width: 120,
                                child: TextField(
                                  controller: distanceController,
                                  keyboardType: TextInputType.number,
                                  decoration: InputDecoration(
                                    border: OutlineInputBorder(),
                                  ),
                                ),
                              )
                            : Text(
                                '$distance',
                                style: TextStyle(
                                    fontSize: 14, fontWeight: FontWeight.w400),
                              ),
                      ],
                    ),
                    const SizedBox(
                      height: 10,
                    ),
                    Row(
                      mainAxisAlignment: MainAxisAlignment.center,
                      children: [
                        if (!isEditing)
                          ElevatedButton(
                            onPressed: () {
                              setState(() {
                                isEditing = true;
                              });
                            },
                            child: Text('Edit'),
                          )
                        else
                          ElevatedButton(
                            onPressed: updateReading,
                            child: Text('Update'),
                          ),
                      ],
                    ),
                  ],
                ),
              ),
            ),
          ],
        ),
      ),
    );

  }
}
