
import { initializeApp } from "firebase/app";

import { getAuth } from 'firebase/auth'

// web app's Firebase configuration
const firebaseConfig = {
  apiKey: "AIzaSyCV108hhimc4xB4uigHBWpKXLcjzXAID1c",
  authDomain: "mars-rover-ae011.firebaseapp.com",
  projectId: "mars-rover-ae011",
  storageBucket: "mars-rover-ae011.appspot.com",
  messagingSenderId: "294808181263",
  appId: "1:294808181263:web:a42f835d4327c72d13217a"
};
 
// Initialize Firebase
const app = initializeApp(firebaseConfig);  

export const auth = getAuth(app)