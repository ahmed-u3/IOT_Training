import 'package:firebase_auth/firebase_auth.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:project_2/house_garage.dart';
import 'package:project_2/house_options_page.dart';


class HomePage extends StatefulWidget{
  const HomePage({Key ? key}) : super(key:key);

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage>{

  final user = FirebaseAuth.instance.currentUser!;
  final _database = FirebaseDatabase.instance.reference();
  List<String> sensors = [];
  List<String> sensors_names = [];
  List<dynamic> sensors_values = [];


  Future getsensors() async{

    await FirebaseFirestore.instance.collection('UsersData').get().then(
      (snapshot) => snapshot.docs.forEach(
        (sensor){
          print(sensor.reference);
          sensors.add(sensor.toString());
        },
      ),
    );
  }

  @override
  void initState(){
    super.initState();
    _activateListeners();
  }

  Future _activateListeners() async{
    _database.child('UsersData').child('Home').onValue.listen((event) {
      Map<String,dynamic> description = event.snapshot.value! as Map<String,dynamic>;
      setState((){
        sensors_names = description.keys.toList();
        sensors_values = description.values.toList();
      });
      //print(sensors_values);
    });
  }

  @override
  Widget build(BuildContext context){
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Colors.deepPurple[200],
        title: Text(
          user.email!,
          style: TextStyle(fontSize: 16),
          ),
          centerTitle: true,
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
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            /*MaterialButton(
              onPressed: (){
                FirebaseAuth.instance.signOut();
              },
              color: Colors.deepPurple[200],
              child: Text('sign out'),
            ),*/
            Expanded(
              child: FutureBuilder(
                future: _activateListeners(), 
                builder: (context,snapshot){
                  return ListView.builder(
                    itemCount: sensors_names.length,
                    itemBuilder: (context,index){
                      return Padding(
                        padding: const EdgeInsets.all(8.0),
                        child: ListTile(
                          title: Text('${sensors_names[index]} : ${sensors_values[index]}'),
                          tileColor: Colors.grey[200],
                        ),
                      );
                    },
                  );
                },
              ),
            ),
          ],
        ),
      )
    );
  }
}