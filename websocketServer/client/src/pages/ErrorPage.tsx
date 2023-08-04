import { isRouteErrorResponse, useRouteError } from 'react-router-dom';


export default function ErrorPage() {
    const error = useRouteError()
    
    if (isRouteErrorResponse(error)) {
        return (
          <div className='flex flex-col justify-center items-center h-full'>
            <h1 className='text-2xl font-bold'>Oops! {error.status}</h1>
            <p>{error.statusText}</p>
            {error.data?.message && (
              <p>
                <i>{error.data.message}</i>
              </p>
            )}
          </div>
        );
      } else if (error instanceof Error) {
        return (
            <div className='flex flex-col justify-center items-center h-full'>
                <h1 className='text-2xl font-bold'>Oops! Unexpected Error</h1>
                <p>Something went wrong.</p>
                <p>
                    <i>{error.message}</i>
                </p>
            </div>
        );
      } else {
        return <></>;
      }
}