/*
 Excercises :-
 - Add button to decrement counter
 - Disallow counter going below zero
 - Add props for lower and upper bound
 */

type state = {count: int};

let initialValue = 0;

[@react.component]
let make = () => {
  let (count, setCount) = React.useState(() => initialValue);

  let handleClick = _ => setCount(_ => count + 1);

  <>
    <h1 className="text-center mb-4 mt-4"> {"Counter" |> React.string} </h1>
    <div className="mx-auto flex w-1/2 justify-center items-center">
      <p className="mr-4"> {count |> string_of_int |> React.string} </p>
      <button
        onClick=handleClick
        className="hover:bg-blue-600 hover:text-white py-1 px-4 border border-blue-600">
        {"+" |> React.string}
      </button>
    </div>
  </>;
};