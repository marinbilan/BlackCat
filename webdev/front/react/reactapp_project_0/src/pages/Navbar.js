import {Link} from 'react-router-dom';

export const Navbar = () => {
    return (
        <div>
        NAVBAR
        <Link to="/"> Home </Link>
        <Link to="/profile"> Profile </Link>
        <Link to="/contacts"> Contacts </Link>
      </div>);
}