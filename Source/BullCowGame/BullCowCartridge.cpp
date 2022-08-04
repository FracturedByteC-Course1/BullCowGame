// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include"HiddenWordList.h"


void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords(Words);

    SetupGame();
}

void UBullCowCartridge::InitGame()
{
    HiddenWord = TEXT("cake");
    Lives = 4;
}

void UBullCowCartridge::SetupGame(){
    PrintLine(TEXT("Welcome to Bull Cows!"));

    HiddenWord = GetValidWords(Words)[FMath::RandRange(0, GetValidWords(Words).Num() - 1)];
  
    Lives = HiddenWord.Len() * 2;
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives."), Lives);
    PrintLine(TEXT("Type in your guess and \npress enter to continue...")); // Prompt Player For Guess
    PrintLine(TEXT("The HiddenWord is: %s."), *HiddenWord);// Debug Line  
}

TArray<FString> UBullCowCartridge::FilterLength(TArray<FString> WordList) const
{
    TArray<FString> RightLengthWords;

    for (int32 Index = 0; Index < WordList.Num(); Index++)
    {
        if (WordList[Index].Len() >= 4 && WordList[Index].Len() <= 8)
        {
            RightLengthWords.Emplace(WordList[Index]);
        }
    }

    PrintLine(TEXT("Words of right length: %i."), RightLengthWords.Num());
    return RightLengthWords;
}

TArray<FString> UBullCowCartridge::FilterForIsograms(TArray<FString> WordList) const
{
    TArray<FString> ValidWords;

    for (int32 Index = 0; Index < WordList.Num(); Index++)
    {
        if (IsIsogram(WordList[Index]))
        {
            ValidWords.Emplace(WordList[Index]);
        }
    }

    PrintLine(TEXT("Number of isograms: %i."), ValidWords.Num());
    return ValidWords;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again."));
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
        for (int32 Index = 0, Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }

    return true;
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{

    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("You have Win!"));
        EndGame();
        return;
    }

    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The hidden word is %i letters long."), HiddenWord.Len());
        PrintLine(TEXT("Sorry, try guessing again! \nYou have %i lives remaining."), Lives);
        return;
    }

    // Check If Isogram
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again!"));
        return;
    }

    // Remove Life
    PrintLine(TEXT("Lost a life!"));
    --Lives;
    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        EndGame();
        return;
    }
    // Show the player Bulls and Cows
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

    PrintLine(TEXT("Guess again, you have %i lives left"), Lives);
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else // Checking PlayerGuess
    {
        ProcessGuess(PlayerInput);
    }
}