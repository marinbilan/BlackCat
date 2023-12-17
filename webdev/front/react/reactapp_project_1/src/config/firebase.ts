// Import the functions you need from the SDKs you need
import { initializeApp } from "firebase/app";
import { getAuth, GoogleAuthProvider } from "firebase/auth";    
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
const firebaseConfig = {
    apiKey: "AIzaSyDvIYDfJX27Xtr3oihlJUOads2pveJ3JQI",
    authDomain: "react-course-11051.firebaseapp.com",
    projectId: "react-course-11051",
    storageBucket: "react-course-11051.appspot.com",
    messagingSenderId: "357653200869",
    appId: "1:357653200869:web:a39a190f24bd4d13fff90c"
  };
  
  // Initialize Firebase
  const app = initializeApp(firebaseConfig);

  export const auth = getAuth(app);
  export const provider = new GoogleAuthProvider();