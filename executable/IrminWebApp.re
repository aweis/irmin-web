open Js_of_ocaml;
module Mem_store = Irmin_mem.KV(Irmin.Contents.String);

let config = Irmin_mem.config();

let repo = Mem_store.Repo.v(config);

open Lwt.Infix;
let master = config => Mem_store.Repo.v(config) >>= Mem_store.master;

let info = msg => {
  let date = Int64.zero;
  let author = "Adam Weis";
  Irmin.Info.v(~date, ~author, msg);
};

let pp_commit_info = (info: Irmin.Info.t) =>
  Irmin.Info.author(info)
  ++ ": "
  ++ Irmin.Info.message(info)
  ++ ": "
  ++ Int64.to_string(Irmin.Info.date(info));

let infof = fmt => Fmt.kstrf((str, ()) => info(str), fmt);

let js_array_of_list = ls => ls |> Array.of_list |> Js.array;

let convertStrings = ls => ls |> List.map(x => Js.string(x));

let jsStringArrayToStringList = jsArray =>
  jsArray
  |> Js.array_map(x => Js.to_string(x))
  |> Js.to_array
  |> Array.to_list;

let getBranches = cb =>
  Lwt.async(() =>
    Mem_store.Repo.v(config)
    >>= Mem_store.Branch.list
    >|= convertStrings
    >|= js_array_of_list
    >|= (t => cb(t))
  );

let set = (key, value, commitMessage, cb) =>
  Mem_store.Repo.v(config)
  >>= Mem_store.master
  >>= (
    t =>
      cb(
        Mem_store.set_exn(
          t,
          key |> jsStringArrayToStringList,
          Js.to_string(value),
          ~info=infof("%s", Js.to_string(commitMessage)),
        ),
      )
  );

let get = (key, cb) =>
  Mem_store.Repo.v(config)
  >>= Mem_store.master
  >>= (
    t =>
      Mem_store.get(t, key |> jsStringArrayToStringList)
      >|= (value => cb(Js.string(value)))
  );

let getHistory = cb =>
  Mem_store.Repo.v(config)
  >>= Mem_store.master
  >>= (
    t =>
      Mem_store.history(t)
      >|= (
        history_res => {
          let str =
            Mem_store.History.fold_vertex(
              (vertex, acc) =>
                acc
                ++ " \n "
                ++ (vertex |> Mem_store.Commit.info |> pp_commit_info),
              history_res,
              "",
            );
          cb(str |> Js.string);
        }
      )
  );

Js.export(
  "Irmin",
  [%js
    {
      val getBranches = getBranches;
      val set = set;
      val get = get;
      val getHistory = getHistory;
      val zero = 0.
    }
  ],
);

let main =
  Mem_store.Repo.v(config)
  >>= Mem_store.master
  >>= (
    t =>
      Mem_store.history(t)
      >|= (
        history_res =>
          Printf.printf(
            "0. is the history empty? %b\n",
            Mem_store.History.is_empty(history_res),
          )
      )
      >>= (
        () => {
          print_endline("setting [a,b,c]");
          Mem_store.set_exn(
            t,
            ["a", "b", "c"],
            "Message 1",
            ~info=infof("COMMIT 1"),
          );
        }
      )
      >>= (
        () =>
          Mem_store.get(t, ["a", "b", "c"])
          >|= (s => print_endline("data at [a,b,c] is " ++ s))
      )
      >>= (
        () =>
          Mem_store.history(t)
          >|= (
            history_res =>
              Printf.printf(
                "1. is the history empty? %b\n",
                Mem_store.History.is_empty(history_res),
              )
          )
      )
      >>= (
        () => {
          print_endline("setting [a,b,d]");
          Mem_store.set_exn(
            t,
            ["a", "b", "d"],
            "Message 2",
            ~info=infof("COMMIT 2"),
          );
        }
      )
      >>= (
        () =>
          Mem_store.get(t, ["a", "b", "c"])
          >|= (s => print_endline("data at [a,b,c] is " ++ s))
      )
      >>= (
        () =>
          Mem_store.get(t, ["a", "b", "d"])
          >|= (s => print_endline("data at [a,b,d] is " ++ s))
      )
      >>= (
        () =>
          Mem_store.history(t)
          >|= (
            history_res =>
              Printf.printf(
                "2. is the history empty? %b\n",
                Mem_store.History.is_empty(history_res),
              )
          )
      )
      >>= (
        () =>
          Mem_store.history(t)
          >|= (
            history_res =>
              Printf.printf(
                "3. number of vertices %d\n number of edges %d\n",
                Mem_store.History.nb_vertex(history_res),
                Mem_store.History.nb_edges(history_res),
              )
          )
      )
      >>= (
        () =>
          Mem_store.history(t)
          >|= (
            history_res =>
              Mem_store.History.iter_vertex(
                v =>
                  print_endline(
                    "vertex! "
                    ++ (v |> Mem_store.Commit.info |> pp_commit_info),
                  ),
                history_res,
              )
          )
      )
  );
