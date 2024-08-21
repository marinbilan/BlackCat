import { useQuery } from "@tanstack/react-querry";
import Axios from "axios";


export const useGetCat = () => {
    const { 
        data, 
        refetch, 
        isLoading: isCatLoading,
        error 
    } = useQuery(["cat"], async () => {
        return Axios.get("https://catfact.ninja/fact").then((res) =>res.data );
    })

    const refetchData = () => {
        alert("DATE REFETCHED");
        refetch();
    }

    if (error) {
        // .. in case of error
    }

    // What we want to get back? Un this case we use object
    // 1. data - 
    // 2. some sort of functionality - func which will refatch if data changed

    return { data, refetchData, isCatLoading };
}