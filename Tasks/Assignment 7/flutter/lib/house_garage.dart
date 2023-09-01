import 'dart:async';

import 'package:firebase_auth/firebase_auth.dart';
import 'package:flutter/material.dart';
import 'package:project_2/auth_page.dart';
//import 'package:google_fonts/google_fonts.dart';
import 'package:project_2/forgot_pw_page.dart';
import 'package:project_2/garage_page.dart';
import 'package:project_2/graph.dart';
import 'package:project_2/home_page.dart';
import 'package:project_2/house_options_page.dart';
import 'package:project_2/login_page.dart';
import 'package:project_2/servo_page.dart';


class HouseGarage extends StatefulWidget{
  //final VoidCallback showRegisterPage;
  const HouseGarage({Key ? key}) : super(key:key);

  @override
  State<HouseGarage> createState() => _HouseGarageState();
}

class _HouseGarageState extends State<HouseGarage>{

  /*Future signIn() async{
    await FirebaseAuth.instance.signInWithEmailAndPassword(
      email: _emailController.text.trim(),
      password: _passwordController.text.trim(),
    );
  }*/

  

  

  @override
  Widget build(BuildContext context){
    return Scaffold(
      appBar: AppBar(
        title: Text('Options',
        style: TextStyle(fontSize: 16),
        ),
        centerTitle: true,
        backgroundColor: Colors.deepPurple[200],
        leading: 
          GestureDetector(
            onTap: () {
              //MaterialButton(
              //onPressed: (){
                FirebaseAuth.instance.signOut();
              //},
              //color: Colors.deepPurple[200],
              //child: Text('sign out')
            //);
            },
            child:Icon(Icons.logout),

          ),
          
      ),
      backgroundColor: Colors.grey[300],
      body: SafeArea(
        child: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              Padding(
                padding: const EdgeInsets.symmetric(horizontal: 25.0),
                child: GestureDetector(
                  onTap: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(
                        builder: (context){
                          return GaragePage();
                        }
                        ),
                      );
                  },
                  child: SizedBox(
                    width: 200,
                    child: Container(
                      padding: EdgeInsets.all(20),
                      decoration: BoxDecoration(
                        color: Colors.deepPurple,
                        borderRadius: BorderRadius.circular(12)
                      ),
                      child: const Center(
                        child: Text(
                          'Garage',
                          style: TextStyle(
                            color: Colors.white,
                            fontWeight: FontWeight.bold,
                            fontSize: 18,
                  
                            ),
                        ),
                      ),
                    ),
                  ),
                ),
                ),

                SizedBox(height: 30,),

                Padding(
                padding: const EdgeInsets.symmetric(horizontal: 25.0),
                child: GestureDetector(
                  onTap: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(
                        builder: (context){
                          return HouseOptions();
                        }));
                        },
                  child: SizedBox(
                    width: 200,
                    child: Container(
                      padding: EdgeInsets.all(20),
                      decoration: BoxDecoration(
                        color: Colors.deepPurple,
                        borderRadius: BorderRadius.circular(12)
                      ),
                      child: const Center(
                        child: Text(
                          'House',
                          style: TextStyle(
                            color: Colors.white,
                            fontWeight: FontWeight.bold,
                            fontSize: 18,
                  
                            ),
                        ),
                      ),
                    ),
                  ),
                ),
                ),

                SizedBox(height: 30,),

                Padding(
                padding: const EdgeInsets.symmetric(horizontal: 25.0),
                child: GestureDetector(
                  onTap: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(
                        builder: (context){
                          return ServoPage();
                        }));
                        },
                  child: SizedBox(
                    width: 200,
                    child: Container(
                      padding: EdgeInsets.all(20),
                      decoration: BoxDecoration(
                        color: Colors.deepPurple,
                        borderRadius: BorderRadius.circular(12)
                      ),
                      child: const Center(
                        child: Text(
                          'Servo',
                          style: TextStyle(
                            color: Colors.white,
                            fontWeight: FontWeight.bold,
                            fontSize: 18,
                  
                            ),
                        ),
                      ),
                    ),
                  ),
                ),
                ),

                SizedBox(height: 30,),

                Padding(
                padding: const EdgeInsets.symmetric(horizontal: 25.0),
                child: GestureDetector(
                  onTap: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(
                        builder: (context){
                          return FetchData();
                        }));
                        },
                  child: SizedBox(
                    width: 200,
                    child: Container(
                      padding: EdgeInsets.all(20),
                      decoration: BoxDecoration(
                        color: Colors.deepPurple,
                        borderRadius: BorderRadius.circular(12)
                      ),
                      child: const Center(
                        child: Text(
                          'Graph',
                          style: TextStyle(
                            color: Colors.white,
                            fontWeight: FontWeight.bold,
                            fontSize: 18,
                  
                            ),
                        ),
                      ),
                    ),
                  ),
                ),
                ),
                
            ],
          ),
        ),
      ),
    );
  }
  }