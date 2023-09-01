import 'dart:async';

import 'package:firebase_auth/firebase_auth.dart';
import 'package:flutter/material.dart';
import 'package:project_2/auth_page.dart';
//import 'package:google_fonts/google_fonts.dart';
import 'package:project_2/forgot_pw_page.dart';
import 'package:project_2/garage_page.dart';
import 'package:project_2/home_control.dart';
import 'package:project_2/home_page.dart';
import 'package:project_2/house_garage.dart';
import 'package:project_2/login_page.dart';


class HouseOptions extends StatefulWidget{
  //final VoidCallback showRegisterPage;
  const HouseOptions({Key ? key}) : super(key:key);

  @override
  State<HouseOptions> createState() => _HouseOptionsState();
}

class _HouseOptionsState extends State<HouseOptions>{

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
        title: Text('House Options',
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
                          return HouseGarage();
                        }));
                        },
            child:Icon(Icons.arrow_back_ios_new),

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
                          return HomePage();
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
                          'Display',
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
                          return HomeControlPage();
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
                          'Control',
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