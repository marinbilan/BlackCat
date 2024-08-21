import { useState } from "react";


export const useToggle = (initialValue = false) => {
    // const [isVisible, setIsVisible] = useState(false);
    const [state, setState] = useState(initialValue);

    const toggle = () => {
        setState( (prev) => !prev );
    }

    // In Hooks we are not returning UI, but logic
    // Can return function, arg, state (if I want to access state outside)
    // So return array (state, function)
    return [state, toggle];
}