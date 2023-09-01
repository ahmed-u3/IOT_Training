import 'main_page.dart';
import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';


void main() async{
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp(
    options: const FirebaseOptions(
      apiKey: "AIzaSyDnTrfYM4_iSAo20kcp80IBikoqxDcNPzo",
      appId: "1:985113887683:android:c64b7ead670ff1b4faf376",
      messagingSenderId: "YOUR_MESSAGING_SENDER_ID",
      projectId: "esp-32-2c84d",
      databaseURL: "https://esp-32-2c84d-default-rtdb.firebaseio.com/",
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