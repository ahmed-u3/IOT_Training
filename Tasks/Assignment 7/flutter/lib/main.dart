import 'main_page.dart';
import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';


void main() async{
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp(
    options: const FirebaseOptions(
      apiKey: "AIzaSyDD-7C9kdGRVVSmdrEBtA3ayJayKpjCQ7w",
      appId: "1:937404884148:android:643b4d4941d60d5b0463e1",
      messagingSenderId: "YOUR_MESSAGING_SENDER_ID",
      projectId: "iotproject-ae710",
      databaseURL: "https://iotproject-ae710-default-rtdb.europe-west1.firebasedatabase.app/",
    ),
  );
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key:key);

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return const MaterialApp(
      debugShowCheckedModeBanner: false,
      home: MainPage(),
    );
  }
}