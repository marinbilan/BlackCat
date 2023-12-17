import { Link } from "react-router-dom"
import { auth } from "../config/firebase"
import { useAuthState } from "react-firebase-hooks/auth"
import { signOut } from "firebase/auth"


export const Navbar = () => {
    const [user] = useAuthState(auth);

    const signUserOut = async () => {
        await signOut(auth);
    }

    return (
    <div> 
        <Link to="/"> HOME </Link>
        <Link to="/login"> LOGIN </Link>
        <div>
            <p> Test Test ... </p>
            <p> { user?.displayName } </p>
            <img src={ user?.photoURL || ""} width="100" height="100" />
        </div>
    </div>);
}