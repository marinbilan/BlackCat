import React from "react"
import {useState, useEffect} from "react";

export const Text = () => {
    // State inside component
    const [text, setText] = useState("");

    useEffect(()=> {
        console.log("COMPONENT MOUNTED");
    });

    return(
        <div>
            <input onChange={ (event) => {
                setText(event.target.value);
            }}
            />

            <h1> {text} </h1>
        </div>    
    );
};