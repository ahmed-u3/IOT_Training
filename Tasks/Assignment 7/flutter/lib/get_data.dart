

import 'package:firebase_auth/firebase_auth.dart';
import 'package:flutter/material.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:firebase_database/firebase_database.dart';

class GetData extends StatefulWidget {
  final String documentId;

  GetData(this.documentId);

  @override
  _GetDataState createState() => _GetDataState();
}

class _GetDataState extends State<GetData> {
  DatabaseReference usersRef = FirebaseDatabase.instance.reference().child('users').child('cx').child('readings');
  Map<String, dynamic>? data;

  @override
  void initState() {
    super.initState();
    usersRef.child(widget.documentId).onValue.listen((event) {
      if (event.snapshot.value != null) {
        setState(() {
          data = event.snapshot.value! as Map<String,dynamic>;
        });
      }
    });
  }

  @override
  Widget build(BuildContext context) {
    if (data != null) {
      return Text(
        '${data!['irsensor']}',
      );
    } else {
      return Text('Loading...');
    }
  }

  @override
  void dispose() {
    usersRef.child(widget.documentId).onValue.drain();
    super.dispose();
  }
}


/*class GetData extends StatelessWidget{

  final String documentId;

  @override
  Widget build(BuildContext context){
    CollectionReference users = FirebaseFirestore.instance.collection('users');

    return FutureBuilder<DocumentSnapshot>{
      future: users.doc(documentId).get(),
      builder: ((context,snapshot){
        if(snapshot.connectionState == ConnectionState.done){
          Map<String,dynamic> data = snapshot.data!.data() as Map<String,dynamic>;
          return Text('${data['first name']}'+
          ' '+
          '${data['last name']},'+
          ' '+
          '${data['age']}' +
          ' years old');


        }
        return Text('loading..');
      }),
    };

  }
}*/